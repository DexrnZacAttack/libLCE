/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Read, Seek, SeekFrom};
use std::vec;
use byteorder::{ByteOrder, LittleEndian, ReadBytesExt};
use crate::compression::get_console_decompression_function;
use crate::{ConsoleType};
use crate::world::block::AquaticBlock;

/*
 * Thanks a lot to UtterEvergreen1 for assisting so much with this!
 * Most of my knowledge on the chunk format is from them.
 */

#[derive(Default, Debug)]
pub struct ConsoleCompressedChunk {
    pub uses_rle: bool,
    pub unknown: bool,
    pub compressed_size: u32, // compressed chunk size
    pub decompressed_size: u32, // fully decompressed size
    pub console_decompressed_size: u32, // after console-specific decompression but before RLE decompression, PS3 only.
    pub chunk: Vec<u8>
}

#[derive(Default, Debug)]
pub struct AquaticSection {
    pub max_section_address: u16,
    pub jump_table: Vec<u16>,
    pub size_table: Vec<u8>,
    pub blocks: Vec<AquaticBlock>,
}

#[derive(Default, Debug)]
pub struct Chunk {
    pub version: i16,
    pub location_x: i32,
    pub location_z: i32,
    pub last_update: i64,
    pub inhabited_time: i64,
    pub section: Vec<AquaticSection>
}

#[derive(Default, Debug)]
pub struct GridIndexEntry {
    format_id: u8,
    offset: u16
}

// wow
pub fn decompress_console_compressed_chunk(reader: &mut Vec<u8>, console_type: ConsoleType) -> std::io::Result<Vec<u8>> {
    get_console_decompression_function(console_type)(reader)
}

pub fn read_grid_index_entry(value: u16) -> GridIndexEntry {
    let format_id = ((value >> 12) & 0xF) as u8;
    let offset = ((value >> 4) & 0xFF) * 4;

    GridIndexEntry {
        format_id,
        offset,
    }
}

// UNFINISHED
pub fn read_chunk<R: Read + Seek + std::fmt::Debug, B: ByteOrder + 'static>(mut reader: R) -> Chunk {
    let mut chunk = Chunk::default();

    // header
    chunk.version = reader.read_i16::<B>().unwrap();
    chunk.location_x = reader.read_i32::<B>().unwrap();
    chunk.location_z = reader.read_i32::<B>().unwrap();
    chunk.last_update = reader.read_i64::<B>().unwrap();
    if (chunk.version > 8) {
        chunk.inhabited_time = reader.read_i64::<B>().unwrap();
    }

    if (chunk.version == 12) {
        let mut section = AquaticSection::default();
        section.max_section_address = reader.read_u16::<B>().unwrap() * 256;

        let mut jump_table: [u16; 16] = [0; 16];
        for i in 0..16 {
            jump_table[i] = reader.read_u16::<B>().unwrap();
        }
        section.jump_table = jump_table.to_vec();

        println!("jump_table: {:?}", section.jump_table);

        let mut size_table: [u8; 16] = [0; 16];
        reader.read_exact(&mut size_table);
        section.size_table = size_table.to_vec();

        let mut sectionData: Vec<Vec<AquaticBlock>> = Vec::new();

        let mut jump_index = 0;
        for jump in jump_table.iter() {
            if (size_table[jump_index] == 0) {
                jump_index += 1;
                continue;
            }

            reader.seek(SeekFrom::Start(0x4c + *jump as u64)).unwrap();
            println!("before grid index: {}", reader.stream_position().unwrap());
            let mut grid_index: Vec<GridIndexEntry> = Vec::new();
            for _ in 0..64 {
                grid_index.push(read_grid_index_entry(reader.read_u16::<B>().unwrap()));
            }

            println!("Grid index: {:?}", grid_index);
            let mut blocks: Vec<AquaticBlock> = Vec::new();

            let reader_pos = reader.stream_position().unwrap();

            for index in grid_index {
                reader.seek(SeekFrom::Start((0x4c + index.offset as u64)));

                match (index.format_id) {
                    0x0 => {
                        let block = reader.read_u16::<B>().unwrap();
                        for _ in 0..64 {
                            blocks.push(AquaticBlock::from_u16(block, std::any::TypeId::of::<B>() == std::any::TypeId::of::<LittleEndian>()));
                        }
                    },
                    _ => {}
                }
            }


            section.blocks = blocks;

            jump_index += 1;
        }
        println!("Section: {:?}", section);
        chunk.section.push(section);
    }

    chunk
}

pub fn read_console_compressed_chunk<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R, console_type: ConsoleType) -> ConsoleCompressedChunk {
    let mut comp_chunk = ConsoleCompressedChunk::default();

    println!("streampos: {:?}", reader.stream_position());
    let comp_size = reader.read_u32::<B>().unwrap();
    println!("Compressed chunk size: {}", comp_size);

    comp_chunk.uses_rle = comp_size & 1 == 1;
    comp_chunk.unknown = (comp_size >> 1) & 1 == 1;
    comp_chunk.compressed_size = comp_size & 0x3FFFFFFF;
    println!("Compressed chunk size (after and): {}", comp_chunk.compressed_size);
    comp_chunk.decompressed_size = reader.read_u32::<B>().unwrap();
    if let ConsoleType::PS3 = console_type {
        comp_chunk.console_decompressed_size = reader.read_u32::<B>().unwrap();
    }
    println!("{}", comp_chunk.compressed_size);
    comp_chunk.chunk = vec![0; (comp_chunk.compressed_size + 2) as usize]; // TEMP: PLEASE DONT PASS +2 NORMALLY, FOR SOME REASON THE SIZE DOESNT INCLUDE ZLIB HEADER
    reader.read_exact(&mut comp_chunk.chunk).unwrap();

    comp_chunk
}