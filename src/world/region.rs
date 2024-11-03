/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::fs::File;
use std::io::{Read, Seek, SeekFrom, Write};
use byteorder::{ByteOrder, ReadBytesExt};
use crate::compression::chunk::decompress_chunk;
use crate::ConsoleType;
use crate::world::chunk::{decompress_console_compressed_chunk, read_console_compressed_chunk, Chunk, ConsoleCompressedChunk};

#[derive(Default, Debug)]
pub struct RegionLocation {
    offset: u32,
    size: u32
}

#[derive(Default, Debug)]
pub struct Region {
    locations: Vec<RegionLocation>,
    timestamps: Vec<u32>,
    chunks: Vec<ConsoleCompressedChunk>,
}

pub fn read_region<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R, console_type: ConsoleType) -> Region {
    let mut region = Region::default();

    for _ in 0..1024 {
        let offset = reader.read_u24::<B>().unwrap();
        println!("{}", offset * 4096);
        let size = reader.read_u8().unwrap();
        region.locations.push(RegionLocation {offset: offset * 4096, size: size as u32 * 4096})
    }

    for _ in 0..1024 {
        let timestamp = reader.read_u32::<B>().unwrap();
        region.timestamps.push(timestamp);
    }

    let mut num = 0;

    for location in region.locations.iter() {
        if (location.offset != 0 && location.size != 0) {
            reader.seek(SeekFrom::Start(location.offset as u64)).unwrap();
            println!("{}", num);
            let mut console_compressed_chunk = read_console_compressed_chunk::<_, B>(&mut reader, console_type);

            // test code
            let console_decompressed_chunk = decompress_console_compressed_chunk(&mut console_compressed_chunk.chunk, ConsoleType::PS3);
            let decompressed_chunk = decompress_chunk(&mut console_decompressed_chunk.unwrap());

            let filename = format!("./testChunks/test{}.ch", num);
            let mut file = File::create(&filename).unwrap();
            
            file.write_all(&decompressed_chunk.unwrap()).unwrap();

            region.chunks.push(console_compressed_chunk);
            num += 1;
        }
    }

    region
}