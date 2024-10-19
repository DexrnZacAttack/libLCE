/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io;
use byteorder::ByteOrder;

// cuz you can't read utf16 string from [u8] or whatever it is (there's like 90 different array types help)
pub(crate) fn u8_to_u16_array<B: ByteOrder>(array: &[u8]) -> io::Result<Vec<u16>> {
    let mut u16_array = Vec::new();

    for chunk in array.chunks(2) {
        if chunk.len() == 2 {
            let value = B::read_u16(chunk);
            u16_array.push(value);
        }
    }

    Ok(u16_array)
}