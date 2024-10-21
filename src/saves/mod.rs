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
use crate::util::string::{write_utf16_padded};
use flate2::write::ZlibEncoder;
use flate2::Compression;

#[derive(Default, Debug)]
pub struct Save {
    file_count: u32,
    index: Vec<SaveIndexFile>
}

pub enum SaveCompression {
    ZLIB,
    LZX,
    VITARLE,
}

pub fn read_save<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> Save {
    let mut save = Save::default();
    let index_offset = reader.read_u32::<B>().unwrap();
    let index_count = reader.read_u32::<B>().unwrap();
    save.file_count = index_count;
    println!("Offset: {:?}, Count: {:?}", index_offset, index_count);

    for i in 0..index_count {
        reader.seek(SeekFrom::Start((index_offset + (144 * i)) as u64)).unwrap();

        // why do I have to do so much jank to read a UTF16 string from a byte array
        let name = util::string::read_utf16::<_, B>(&mut reader, 0x80);
        let size: u32 = reader.read_u32::<B>().unwrap();
        let offset = reader.read_u32::<B>().unwrap();
        let timestamp = reader.read_u64::<B>().unwrap();

        let mut buffer = vec![0u8; size as usize];
        reader.seek(SeekFrom::Start(offset as u64)).unwrap();
        reader.read_exact(&mut *buffer).unwrap();

        println!("Name: {}, Timestamp: {:?}, Offset: {:?}, Size: {:?}", name, timestamp, offset, size);
        save.index.push(SaveIndexFile {name: name.trim_end_matches('\0').parse().unwrap(), size, offset, timestamp, data: buffer});
    }
    save
}

pub(crate) fn write_save<B: ByteOrder>(mut files: Vec<BasicFile>, minimum_version: u16, current_version: u16) -> BasicFile {
    let mut writer = Cursor::new(Vec::new());
    writer.write_u32::<B>(0).unwrap(); // write temp offset
    writer.write_u32::<B>(files.len() as u32).unwrap(); // file count
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
        writer.write_u64::<B>(SystemTime::now().duration_since(UNIX_EPOCH).expect("Time went backwards, are you a time traveler?").as_secs()).unwrap();
    }

    let mut data = Vec::new();
    // janky, read to the end to get the data
    writer.seek(SeekFrom::Start(0)).unwrap();
    writer.read_to_end(&mut data).unwrap();

    let size = writer.get_ref().len();
    BasicFile {name: "savegame.dat".parse().unwrap(), size, data}
}

pub(crate) fn compress_save<B: ByteOrder>(save: BasicFile, r#type: SaveCompression) -> BasicFile {
    let mut buffer = Cursor::new(Vec::new());
    buffer.write_u64::<B>(save.size as u64).unwrap();

    if (matches!(r#type, SaveCompression::ZLIB)) {
        let mut compressor = ZlibEncoder::new(buffer, Compression::default());
        compressor.write_all(&save.data).unwrap();
        let compressed = compressor.finish().unwrap().into_inner();
        return BasicFile { name: "gamesave.dat".parse().unwrap(), size: compressed.len(), data: compressed }
    } else {
        panic!("Compression type is not implemented yet.");
    }
}