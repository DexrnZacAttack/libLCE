/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use flate2::read::DeflateDecoder;
use std::io::{Cursor, Read};

pub fn inflate(data: &mut Vec<u8>) -> std::io::Result<Vec<u8>> {
    let mut decoder = DeflateDecoder::new(Cursor::new(data));
    let mut buffer = Vec::new();
    decoder.read_to_end(&mut buffer)?;
    println!("{:?}", buffer);
    Ok(buffer)
}