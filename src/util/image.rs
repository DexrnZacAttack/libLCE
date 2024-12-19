/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use png::Decoder;
use std::io::{Read, Seek};

pub(crate) fn get_raw_pixels_from_png<R: Read + Seek + std::fmt::Debug>(reader: R) -> Vec<u8> {
    let dec = Decoder::new(reader);
    let mut png_reader = dec.read_info().unwrap();
    let mut buf = vec![0; png_reader.output_buffer_size()];
    png_reader.next_frame(&mut buf).unwrap();
    buf
}