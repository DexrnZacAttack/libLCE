/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use png::{BitDepth, ColorType, Decoder, Encoder, ScaledFloat, SourceChromaticities};
use std::io::{Cursor, Read, Seek, SeekFrom};
use byteorder::{ByteOrder};
use png::text_metadata::TEXtChunk;
use serde::Serialize;
use serde_json::Value;
use crate::file::types::BasicFile;
use crate::util::io::rw::stream_to_basic_file;
use crate::util::io::string::{read_utf16_null_terminated, read_utf32_null_terminated, write_utf16_padded, write_utf32_padded};

#[derive(Default, Debug, Serialize)]
pub(crate) struct Thumbnail {
    pub name: String,
    pub world_info: FjText,
    pub thumbnail: Vec<u8>
}

#[derive(Debug, Default, Serialize)]
pub(crate) struct FjText {
    pub fj_seed: String,
    pub fj_host_options: String,
    pub fj_texture_pack: String,
    pub fj_extra_data: String,
    pub fj_base_save_name: String,
    pub fj_loads: String,
    pub fj_explored_chunks: String,
}

pub(crate) fn get_4jtxt_name(key: String) -> &'static str {
    match key.as_str() {
        "fj_seed" => "4J_SEED",
        "fj_host_options" => "4J_HOSTOPTIONS",
        "fj_texture_pack" => "4J_TEXTUREPACK",
        "fj_extra_data" => "4J_EXTRADATA",
        "fj_base_save_name" => "4J_BASESAVENAME",
        "fj_loads" => "4J_#LOADS",
        "fj_explored_chunks" => "4J_EXPLOREDCHUNKS",
        _ => {panic!("unknown key: {}", key)},
    }
}

pub(crate) fn read_thumb<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R, header_size: usize, use_utf32: bool) -> Thumbnail {
    let mut thumb = Thumbnail::default();

    while let Ok(current_position) = reader.stream_position() {
        if current_position < header_size as u64 {
            if (!use_utf32) {
                thumb.name = read_utf16_null_terminated::<_, B>(&mut reader);
            } else {
                thumb.name = read_utf32_null_terminated::<_, B>(&mut reader);
            }
            break;
        } else {
            break;
        }
    }
    reader.seek(SeekFrom::Start(header_size as u64)).unwrap();

    let mut decoder = Decoder::new(reader);
    decoder.set_ignore_text_chunk(false);
    let mut png_reader = decoder.read_info().unwrap();
    let mut buf = vec![0; png_reader.output_buffer_size()];
    png_reader.next_frame(&mut buf).unwrap();
    png_reader.finish();

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
                    _ => {},
                }
            }
        }
    }

    thumb.thumbnail = buf;
    thumb
}

pub(crate) fn write_thumb<B: ByteOrder>(mut thumbnail: Thumbnail, header_size: usize, use_utf32: bool) -> BasicFile {
    let mut writer: Cursor<Vec<u8>> = Cursor::new(Vec::new());
    if (!use_utf32) {
        write_utf16_padded::<_, B>(&mut writer, &*thumbnail.name, (header_size / 2) as u16, true, false);
    } else {
        write_utf32_padded::<_, B>(&mut writer, &*thumbnail.name, (header_size / 4) as u16, true, false);
    }

    {
        let mut encoder = Encoder::new(&mut writer, 64, 64);
        encoder.set_color(ColorType::Rgba);
        encoder.set_depth(BitDepth::Eight);

        let mut img_writer = encoder.write_header().unwrap();
        img_writer.write_image_data(&thumbnail.thumbnail).unwrap();
        let serialized = serde_json::to_value(&thumbnail.world_info).unwrap();
        let mut text_key = String::new();
        let mut text_content = String::new();

        if let Value::Object(map) = serialized {
            for (key, value) in map {
                let fj_key = get_4jtxt_name(key);
                if let Value::String(val) = value {
                    if !val.is_empty() && text_key.is_empty() {
                        text_key = fj_key.parse().unwrap();
                    }

                    if (!val.is_empty()) {
                        if (fj_key != text_key) {
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
            img_writer.write_text_chunk(&chunk).unwrap();
        }
    }

    stream_to_basic_file(&mut writer, "THUMB")
}