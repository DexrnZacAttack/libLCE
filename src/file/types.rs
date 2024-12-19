/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

#[derive(Default, Clone, Debug)]
pub struct BasicFile {
    pub name: String,
    pub size: usize,
    pub data: Vec<u8>,
}

#[derive(Default, Debug, Clone)]
pub struct SaveIndexFile {
    pub name: String,
    pub size: u32,
    pub offset: u32,
    pub timestamp: u64,
    pub data: Vec<u8>,
}

impl From<SaveIndexFile> for BasicFile {
    fn from(file: SaveIndexFile) -> Self {
        BasicFile { name: file.name, size: file.size as usize, data: file.data }
    }
}

impl From<BasicFile> for SaveIndexFile {
    fn from(file: BasicFile) -> Self {
        SaveIndexFile { name: file.name, size: file.size as u32, offset: 0, timestamp: 0, data: file.data }
    }
}