/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use crate::file::types::BasicFile;
use bytestream::{ByteOrder, StreamReader};
use std::io::{Read, Seek, SeekFrom, Write};

pub fn stream_to_basic_file<S: Read + Write + Seek + std::fmt::Debug>(stream: &mut S, file_name: &str) -> BasicFile {
    let mut data = Vec::new();
    // janky, read to the end to get the data
    stream.seek(SeekFrom::Start(0)).unwrap();
    stream.read_to_end(&mut data).unwrap();

    let size = data.len();
    BasicFile { name: file_name.parse().unwrap(), size, data }
}

/// Reads Long if 64 bit, otherwise Int.
///
/// *This does not however (by default) use the system architecture.*
pub fn read_c_ulong<R: Read + Seek + std::fmt::Debug>(mut reader: R, use_u64: bool, byte_order: ByteOrder) -> u64 {
    if use_u64 {
        u64::read_from(&mut reader, byte_order).unwrap()
    } else {
        u32::read_from(&mut reader, byte_order).unwrap() as u64
    }
}