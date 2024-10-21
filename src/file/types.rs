/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

#[derive(Default, Clone, Debug)]
pub struct BasicFile {
    pub(crate) name: String,
    pub(crate) size: usize,
    pub(crate) data: Vec<u8>
}

#[derive(Default, Debug)]
pub struct SaveIndexFile {
    pub(crate) name: String,
    pub(crate) size: u32,
    pub(crate) offset: u32,
    pub(crate) timestamp: u64,
    pub(crate) data: Vec<u8>
}