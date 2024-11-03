/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/
use std::any::Any;
use std::io::{Cursor, Read, Seek, SeekFrom, Write};
use std::time::{SystemTime, UNIX_EPOCH};
use byteorder::{ByteOrder, ReadBytesExt, WriteBytesExt};
use crate::file::types::{BasicFile, SaveIndexFile};
use crate::util;
use crate::util::io::string::{write_utf16_padded};
use flate2::write::ZlibEncoder;
use flate2::Compression;
use crate::compression::CompressionTypes;
use crate::util::io::rw::stream_to_basic_file;

#[derive(Default, Debug)]
pub struct Save {
    file_count: u32,
    index: Vec<SaveIndexFile>
}

enum SaveVersion {
    // version names are equivalent to the first version where the file version is used
    // e.g TU5 is the first version to use file version 3.
    TU0033 = 1,
    TU0054 = 2,
    TU5 = 3,
    TU9 = 5,
    TU14 = 6,
    TU17 = 8,
    TU19 = 9,
    TU36 = 10,
    TU69 = 11
}

pub fn read_save<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> Save {
    let mut save = Save::default();
    let index_offset = reader.read_u32::<B>().unwrap();
    let mut index_count = reader.read_u32::<B>().unwrap();
    let minimum_version = reader.read_u16::<B>().unwrap();
    let current_version = reader.read_u16::<B>().unwrap();

    let mut is_prerelease = false;

    if (current_version == SaveVersion::TU0033 as u16) {
        is_prerelease = true;
        index_count = index_count / 136;
    }

    save.file_count = index_count;
    println!("Offset: {:?}, Count: {:?}", index_offset, index_count);

    for i in 0..index_count {
        reader.seek(SeekFrom::Start((index_offset + (144 * i)) as u64)).unwrap();

        // why do I have to do so much jank to read a UTF16 string from a byte array
        let name = util::io::string::read_utf16::<_, B>(&mut reader, 0x80);
        let size: u32 = reader.read_u32::<B>().unwrap();
        let offset = reader.read_u32::<B>().unwrap();

        let mut timestamp = 0;

        if (!is_prerelease) {
            timestamp = reader.read_u64::<B>().unwrap();
        }

        let mut buffer = vec![0u8; size as usize];
        reader.seek(SeekFrom::Start(offset as u64)).unwrap();
        reader.read_exact(&mut *buffer).unwrap();

        println!("Name: {}, Timestamp: {:?}, Offset: {:?}, Size: {:?}", name, timestamp, offset, size);
        save.index.push(SaveIndexFile {name: name.trim_end_matches('\0').parse().unwrap(), size, offset, timestamp, data: buffer});
    }
    save
}

pub(crate) fn write_save<B: ByteOrder>(mut files: Vec<BasicFile>, minimum_version: u16, current_version: u16) -> BasicFile {
    let mut is_prerelease = false;

    if (current_version == SaveVersion::TU0033 as u16) {
        is_prerelease = true;
    }

    let mut writer = Cursor::new(Vec::new());
    writer.write_u32::<B>(0).unwrap(); // write temp offset
    writer.write_u32::<B>(if is_prerelease { (files.len() * 136) as u32 } else { files.len() as u32 }).unwrap(); // file count
    writer.write_u16::<B>(minimum_version).unwrap();
    writer.write_u16::<B>(current_version).unwrap();

    let mut offsets = Vec::new();

    for i in 0..files.len() {
        offsets.push(writer.stream_position().unwrap() as u32);
        writer.write(&mut files[i].data).unwrap();
    };

    let offset = writer.stream_position().unwrap();
    writer.set_position(0);
    writer.write_u32::<B>(offset as u32).unwrap();
    writer.set_position(offset);

    for i in 0..files.len() {
        write_utf16_padded::<_, B>(&mut writer, &mut files[i].name, 0x40, true, false);
        writer.write_u32::<B>(files[i].data.len() as u32).unwrap();
        writer.write_u32::<B>(offsets[i]).unwrap();
        if (!is_prerelease) {
            writer.write_u64::<B>(SystemTime::now().duration_since(UNIX_EPOCH).expect("Time went backwards, are you a time traveler?").as_secs()).unwrap();
        }
    }

    stream_to_basic_file(&mut writer, "savegame.dat")
}

pub(crate) fn compress_save<B: ByteOrder>(save: BasicFile, r#type: CompressionTypes) -> BasicFile {
    let mut buffer = Cursor::new(Vec::new());
    buffer.write_u64::<B>(save.size as u64).unwrap();

    if (matches!(r#type, CompressionTypes::ZLIB)) {
        let mut compressor = ZlibEncoder::new(buffer, Compression::default());
        compressor.write_all(&save.data).unwrap();
        let compressed = compressor.finish().unwrap().into_inner();
        BasicFile { name: "savegame.dat".parse().unwrap(), size: compressed.len(), data: compressed }
    } else {
        panic!("Compression type is not implemented yet.");
    }
}