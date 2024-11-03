/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

pub mod zlib;
pub mod chunk;
mod deflate;

use std::io::{Read, Seek};
use crate::compression::deflate::inflate;
use crate::compression::zlib::decompress_zlib;
use crate::ConsoleType;

pub enum CompressionTypes {
    LZX,
    ZLIB,
    DEFLATE,
    VITARLE,
    SPLITSAVERLE,
    NONE
}

// hmm these are all really weird, maybe should find a better way of doing these
pub fn get_console_compression(console: ConsoleType) -> CompressionTypes {
    match console {
        ConsoleType::X360 => CompressionTypes::LZX,
        ConsoleType::PS3 => CompressionTypes::DEFLATE,
        ConsoleType::WIIU => CompressionTypes::ZLIB,
        ConsoleType::VITA => CompressionTypes::VITARLE,
        ConsoleType::XONE => CompressionTypes::ZLIB,
        ConsoleType::PS4 => CompressionTypes::ZLIB,
        ConsoleType::NS => CompressionTypes::ZLIB,
        ConsoleType::WIN => CompressionTypes::ZLIB, // UNKNOWN
    }
}

pub fn get_console_decompression_function(console: ConsoleType) -> fn(&mut Vec<u8>) -> std::io::Result<Vec<u8>> {
    match console {
        ConsoleType::X360 => panic!("Compression not implemented."),
        ConsoleType::PS3 => inflate,
        ConsoleType::WIIU => decompress_zlib,
        ConsoleType::VITA => panic!("Compression not implemented."),
        ConsoleType::XONE => decompress_zlib,
        ConsoleType::PS4 => decompress_zlib,
        ConsoleType::NS => decompress_zlib,
        ConsoleType::WIN => decompress_zlib, // UNKNOWN
    }
}