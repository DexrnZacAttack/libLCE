/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Read, Seek, SeekFrom};
use byteorder::{BigEndian, ReadBytesExt};
use crate::{BasicFile};
use crate::util::string::read_utf8_prefixed_size;

#[derive(Default, Debug)]
pub struct Arc {
    file_count: u32,
    index: Vec<BasicFile>
}

pub fn read_arc<R: Read + Seek + std::fmt::Debug>(mut reader: R) -> Arc {
    let mut arc = Arc::default();
    let index_count = reader.read_u32::<BigEndian>().unwrap();
    arc.file_count = index_count;

    for i in 0..index_count {
        let name = read_utf8_prefixed_size::<_, BigEndian>(&mut reader);
        let offset = reader.read_u32::<BigEndian>().unwrap();
        let size: u32 = reader.read_u32::<BigEndian>().unwrap();
        let current_position = reader.stream_position().unwrap();

        let mut buffer = vec![0u8; size as usize];
        reader.seek(SeekFrom::Start(offset as u64)).unwrap();
        reader.read_exact(&mut *buffer).unwrap();

        println!("Name: {}, Offset: {:?}, Size: {:?}", name, offset, size);
        arc.index.push(BasicFile {name, size, offset, data: buffer});
        reader.seek(SeekFrom::Start(current_position)).unwrap();
    }
    arc
}