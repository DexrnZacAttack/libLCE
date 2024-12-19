/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/
use crate::compression::CompressionTypes;
use crate::file::types::{BasicFile, SaveIndexFile};
use crate::util::io::ex_string::{ExStringR, ExStringW};
use crate::util::io::rw::stream_to_basic_file;
use byteorder::{ByteOrder, ReadBytesExt, WriteBytesExt};
use flate2::write::ZlibEncoder;
use flate2::Compression;
use std::io::{Cursor, Error, Read, Seek, SeekFrom, Write};
use std::time::{SystemTime, UNIX_EPOCH};

/// A save file
#[derive(Default, Debug)]
pub struct Save {
    pub file_count: u32,
    pub index: Vec<SaveIndexFile>,
}

/// Save file versions, as far as I know these aren't used for much.
enum SaveVersion {
    // version names are equivalent to the first version where the file version is used
    // e.g. TU5 is the first version to use file version 3.
    TU0033 = 1,
    /// Pre-release save file
    TU0054 = 2,
    TU5 = 3,
    TU9 = 5,
    TU14 = 6,
    TU17 = 8,
    TU19 = 9,
    TU36 = 10,
    TU69 = 11,
}

impl Save {
    /// Reads a save file and returns a Save instance.
    pub fn read<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> Result<Save, Error> {
        let mut save = Save::default();
        println!("{:?}", reader);
        let index_offset = reader.read_u32::<B>()?;
        let mut index_count = reader.read_u32::<B>()?;
        let _minimum_version = reader.read_u16::<B>()?;
        let current_version = reader.read_u16::<B>()?;

        let mut is_prerelease = false;

        if current_version == SaveVersion::TU0033 as u16 {
            is_prerelease = true;
            index_count = index_count / 136;
        }

        save.file_count = index_count;
        println!("Offset: {:?}, Count: {:?}", index_offset, index_count);

        for i in 0..index_count {
            reader.seek(SeekFrom::Start((index_offset + (144 * i)) as u64))?;

            // why do I have to do so much jank to read a UTF16 string from a byte array
            let name = reader.read_utf16::<B>(0x80);
            let size: u32 = reader.read_u32::<B>()?;
            let offset = reader.read_u32::<B>()?;

            let mut timestamp = 0;

            if !is_prerelease {
                timestamp = reader.read_u64::<B>()?;
            }

            let mut buffer = vec![0u8; size as usize];
            reader.seek(SeekFrom::Start(offset as u64))?;
            reader.read_exact(&mut *buffer)?;

            save.index.push(SaveIndexFile { name: name.trim_end_matches('\0').parse().unwrap(), size, offset, timestamp, data: buffer });
        }
        Ok(save)
    }

    /// Returns a BasicFile from a Save which can be written to disk
    pub fn write<B: ByteOrder>(&mut self, minimum_version: u16, current_version: u16) -> Result<BasicFile, Error> {
        let files = &mut self.index;
        let mut is_prerelease = false;

        if current_version == SaveVersion::TU0033 as u16 {
            is_prerelease = true;
        }

        let mut writer = Cursor::new(Vec::new());
        writer.write_u32::<B>(0)?; // write temp offset
        writer.write_u32::<B>(if is_prerelease { (files.len() * 136) as u32 } else { files.len() as u32 })?; // file count
        writer.write_u16::<B>(minimum_version)?;
        writer.write_u16::<B>(current_version)?;

        let mut offsets = Vec::new();

        for i in 0..files.len() {
            offsets.push(writer.stream_position()? as u32);
            writer.write(&mut files[i].data)?;
        };

        let offset = writer.stream_position()?;
        writer.set_position(0);
        writer.write_u32::<B>(offset as u32)?;
        writer.set_position(offset);

        for i in 0..files.len() {
            writer.write_utf16_padded::<B>(&mut files[i].name, 0x40, true, false);
            writer.write_u32::<B>(files[i].data.len() as u32)?;
            writer.write_u32::<B>(offsets[i])?;
            if !is_prerelease {
                writer.write_u64::<B>(SystemTime::now().duration_since(UNIX_EPOCH).expect("Time went backwards, are you a time traveler?").as_secs())?;
            }
        }

        Ok(stream_to_basic_file(&mut writer, "savegame.dat"))
    }

    /// Compresses any given BasicFile the same way saves are compressed
    ///
    /// Not all compression types have been implemented.
    ///
    /// Supported compression types:
    /// - [X] Zlib
    /// - [ ] Raw Deflate
    /// - [ ] LZX (XMemCompress)
    /// - [ ] Vita RLE
    /// - [ ] Split Save RLE (not meant to compress regular saves)
    /// - [ ] Chunk RLE (not meant to compress saves)
    pub fn compress<B: ByteOrder>(save: BasicFile, r#type: CompressionTypes) -> Result<BasicFile, Error> {
        let mut buffer = Cursor::new(Vec::new());
        buffer.write_u64::<B>(save.size as u64)?;

        if matches!(r#type, CompressionTypes::ZLIB) {
            let mut compressor = ZlibEncoder::new(buffer, Compression::default());
            compressor.write_all(&save.data)?;
            let compressed = compressor.finish()?.into_inner();

            Ok(BasicFile { name: "savegame.dat".parse().unwrap(), size: compressed.len(), data: compressed })
        } else {
            panic!("Compression type is not implemented yet.");
        }
    }
}

impl From<Vec<BasicFile>> for Save {
    fn from(value: Vec<BasicFile>) -> Self {
        let mut save = Save::default();

        value.iter().for_each(|file| {
            save.file_count += 1;
            save.index.push(SaveIndexFile::from(file.clone()));
        });

        save
    }
}