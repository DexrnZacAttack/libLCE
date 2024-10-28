/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Seek, Write, Read, SeekFrom};
use crate::file::types::BasicFile;

pub fn stream_to_basic_file<S: Read + Write + Seek + std::fmt::Debug>(stream: &mut S, file_name: &str) -> BasicFile {
    let mut data = Vec::new();
    // janky, read to the end to get the data
    stream.seek(SeekFrom::Start(0)).unwrap();
    stream.read_to_end(&mut data).unwrap();

    let size = data.len();
    BasicFile {name: file_name.parse().unwrap(), size, data}
}