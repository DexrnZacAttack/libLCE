/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Read, Seek, SeekFrom};
use byteorder::{ByteOrder, ReadBytesExt};
use crate::util;

#[derive(Default, Debug)]
pub struct IndexFile {
    name: String,
    size: u32,
    offset: u32,
    timestamp: u64,
    data: Vec<u8>
}
#[derive(Default, Debug)]
pub struct Save {
    file_count: u32,
    index: Vec<IndexFile>
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
        let mut buffer = [0u8; 0x80];
        reader.read_exact(&mut buffer);
        let u16_string = util::array::u8_to_u16_array::<B>(&buffer).unwrap();
        let utf16_string = String::from_utf16(&u16_string).unwrap();
        let name = utf16_string.trim_end_matches('\0').to_string();
        let size: u32 = reader.read_u32::<B>().unwrap();
        let offset = reader.read_u32::<B>().unwrap();
        let timestamp = reader.read_u64::<B>().unwrap();

        let mut buffer = vec![0u8; size as usize];
        reader.seek(SeekFrom::Start(offset as u64)).unwrap();
        reader.read_exact(&mut *buffer).unwrap();

        println!("Name: {}, Timestamp: {:?}, Offset: {:?}, Size: {:?}", name, timestamp, offset, size);
        save.index.push(IndexFile {name, size, offset, timestamp, data: buffer});
    }
    save
}