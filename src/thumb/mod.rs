/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use crate::file::types::BasicFile;
use crate::util::io::ex_string::{ExStringR, ExStringW};
use crate::util::io::rw::stream_to_basic_file;
use byteorder::ByteOrder;
use png::text_metadata::TEXtChunk;
use png::{BitDepth, ColorType, Decoder, Encoder};
use serde::Serialize;
use serde_json::Value;
use std::io::{Cursor, Error, Read, Seek, SeekFrom};

/// The world thumbnail saved on world exit
///
/// Contains
/// * Thumbnail image
/// * World Name (on some platforms)
/// * World Info ([`FjText`])
#[derive(Default, Debug, Serialize)]
pub struct Thumbnail {
    pub name: String,
    pub world_info: FjText,
    pub thumbnail: Vec<u8>,
}

#[derive(Debug, Default, Serialize)]
pub struct FjText {
    pub fj_seed: String,
    pub fj_host_options: String,
    pub fj_texture_pack: String,
    pub fj_extra_data: String,
    pub fj_base_save_name: String,
    pub fj_loads: String,
    pub fj_explored_chunks: String,
}

/// Maps the struct entries to the actual strings found in the tEXt chunk.
pub fn get_4jtxt_name(key: String) -> &'static str {
    match key.as_str() {
        "fj_seed" => "4J_SEED",
        "fj_host_options" => "4J_HOSTOPTIONS",
        "fj_texture_pack" => "4J_TEXTUREPACK",
        "fj_extra_data" => "4J_EXTRADATA",
        "fj_base_save_name" => "4J_BASESAVENAME",
        "fj_loads" => "4J_#LOADS",
        "fj_explored_chunks" => "4J_EXPLOREDCHUNKS",
        _ => { panic!("Unknown key: {}", key) }
    }
}

impl Thumbnail {
    /// Reads a thumbnail (e.g. world.ext or THUMB) and returns the world name (if applicable), thumbnail, and world metadata.
    pub fn read<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R, header_size: usize, use_utf32: bool) -> Result<Thumbnail, Error> {
        let mut thumb = Thumbnail::default();

        while let Ok(current_position) = reader.stream_position() {
            if current_position < header_size as u64 {
                if !use_utf32 {
                    thumb.name = reader.read_utf16_null_terminated::<B>();
                } else {
                    thumb.name = reader.read_utf32_null_terminated::<B>();
                }
                break;
            } else {
                break;
            }
        }
        reader.seek(SeekFrom::Start(header_size as u64))?;

        let mut dec = Decoder::new(reader);
        dec.set_ignore_text_chunk(false);
        let mut png_reader = dec.read_info()?;
        let mut buf = vec![0; png_reader.output_buffer_size()];
        png_reader.next_frame(&mut buf)?;
        png_reader.finish()?;

        for text_chunk in &png_reader.info().uncompressed_latin1_text {
            let kv = (text_chunk.keyword.clone() + "\0") + &*text_chunk.text.clone();
            let kvs: Vec<&str> = kv.split('\0').collect();

            for i in (0..kvs.len()).step_by(2) {
                if i + 1 < kvs.len() {
                    let key = kvs[i].trim();
                    let value = kvs[i + 1].trim().trim_matches('"').to_string();

                    match key {
                        "4J_SEED" => thumb.world_info.fj_seed = value,
                        "4J_HOSTOPTIONS" => thumb.world_info.fj_host_options = value,
                        "4J_TEXTUREPACK" => thumb.world_info.fj_texture_pack = value,
                        "4J_EXTRADATA" => thumb.world_info.fj_extra_data = value,
                        "4J_BASESAVENAME" => thumb.world_info.fj_base_save_name = value,
                        "4J_#LOADS" => thumb.world_info.fj_loads = value,
                        "4J_EXPLOREDCHUNKS" => thumb.world_info.fj_explored_chunks = value,
                        _ => {}
                    }
                }
            }
        }

        thumb.thumbnail = buf;
        
        Ok(thumb)
    }

    /// Returns a BasicFile which can be written to disk
    pub fn write<B: ByteOrder>(&self, header_size: usize, use_utf32: bool) -> Result<BasicFile, Error> {
        let mut writer: Cursor<Vec<u8>> = Cursor::new(Vec::new());
        if !use_utf32 {
            writer.write_utf16_padded::<B>(&self.name, (header_size / 2) as u16, true, false);
        } else {
            writer.write_utf32_padded::<B>(&self.name, (header_size / 4) as u16, true, false);
        }

        {
            let mut enc = Encoder::new(&mut writer, 64, 64);
            enc.set_color(ColorType::Rgba);
            enc.set_depth(BitDepth::Eight);

            let mut img_writer = enc.write_header()?;
            img_writer.write_image_data(&self.thumbnail)?;
            let serialized = serde_json::to_value(&self.world_info)?;
            let mut text_key = String::new();
            let mut text_content = String::new();

            if let Value::Object(map) = serialized {
                for (key, value) in map {
                    let fj_key = get_4jtxt_name(key);
                    if let Value::String(val) = value {
                        if !val.is_empty() && text_key.is_empty() {
                            text_key = fj_key.parse().unwrap();
                        }

                        if !val.is_empty() {
                            if fj_key != text_key {
                                text_content += &fj_key;
                                text_content += "\0";
                            }
                            text_content += &val;
                            text_content += "\0";
                        }
                    }
                }
            }

            if !text_key.is_empty() {
                let chunk = TEXtChunk::new(text_key, text_content);
                img_writer.write_text_chunk(&chunk)?;
            }
        }

        Ok(stream_to_basic_file(&mut writer, "THUMB"))
    }
}