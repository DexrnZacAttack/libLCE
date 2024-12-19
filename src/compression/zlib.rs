/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use flate2::read::ZlibDecoder;
use std::io::{Cursor, Read};
use wasm_bindgen::prelude::wasm_bindgen;

pub fn decompress_zlib(data: &mut Vec<u8>) -> std::io::Result<Vec<u8>> {
    let mut decoder = ZlibDecoder::new(Cursor::new(data));
    let mut buffer = Vec::new();
    decoder.read_to_end(&mut buffer)?;
    println!("{:?}", buffer);
    Ok(buffer)
}