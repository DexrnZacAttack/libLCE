/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

// Thanks to UtterEvergreen1 for helping with chunk compression.
pub fn decompress_chunk(data: &mut Vec<u8>) -> Vec<u8> {
    let mut writer = Vec::new();
    let mut i = 0;

    while i < data.len() {
        let byte = data[i];
        i += 1;

        if byte != 0xFF {
            writer.push(byte);
        } else {
            let mut write_value = 255;
            let num_of_value = data[i];
            i += 1;

            if num_of_value >= 3 {
                write_value = data[i];
                i += 1;
            }

            writer.extend(vec![write_value; num_of_value as usize + 1]);
        }
    }

    writer
}
