/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

#[derive(Default, Debug)]
pub struct AquaticBlock {
    waterlogged: bool,
    id: u16,
    data: u8
}

impl AquaticBlock {
    // got fucking tired so I'm doing it this way, why is it so fucking hard to get the actual type passed wtf
    pub fn from_u16(value: u16, little_endian: bool) -> Self {
        if (little_endian == true) {
            let waterlogged = (value & 0b1000_0000_0000_0000) != 0;
            let id = (value & 0b0111_1111_1111_0000) >> 4;
            let data = (value & 0b0000_0000_0000_1111) as u8;

            AquaticBlock {
                id,
                data,
                waterlogged,
            }
        } else {
            let block_id_lsb = (value & 0b1111_0000_0000_0000) >> 12;
            let data = (value & 0b0000_1111_0000_0000) >> 8;
            let waterlogged = (value & 0b0000_0000_1000_0000) != 0;
            let block_id_msb = (value & 0b0000_0000_0111_1111);
            let id = (block_id_lsb << 7) | block_id_msb;

            AquaticBlock {
                id,
                data: data as u8,
                waterlogged,
            }
        }
    }
}