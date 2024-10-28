/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Read, Seek};
use png::Decoder;

pub(crate) fn get_raw_pixels_from_png<R: Read + Seek + std::fmt::Debug>(mut reader: R) -> Vec<u8> {
    let mut decoder = Decoder::new(reader);
    let mut png_reader = decoder.read_info().unwrap();
    let mut buf = vec![0; png_reader.output_buffer_size()];
    png_reader.next_frame(&mut buf).unwrap();
    buf
}