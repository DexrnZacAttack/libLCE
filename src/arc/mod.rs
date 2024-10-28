/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Cursor, Read, Seek, SeekFrom, Write};
use byteorder::{BigEndian, ReadBytesExt, WriteBytesExt};
use crate::file::types::BasicFile;
use crate::util::io::rw::stream_to_basic_file;
use crate::util::io::string::{read_utf8_prefixed_size, write_utf8};

#[derive(Default, Debug)]
pub struct Arc {
    file_count: u32,
    index: Vec<BasicFile>
}

pub fn read_arc<R: Read + Seek + std::fmt::Debug>(mut reader: R) -> Arc {
    let mut arc = Arc::default();
    let index_count = reader.read_u32::<BigEndian>().unwrap();
    arc.file_count = index_count;

    for i in 0..index_count {
        let name = read_utf8_prefixed_size::<_, BigEndian>(&mut reader);
        let offset = reader.read_u32::<BigEndian>().unwrap();
        let size = reader.read_u32::<BigEndian>().unwrap();
        let current_position = reader.stream_position().unwrap();

        let mut buffer = vec![0u8; size as usize];
        reader.seek(SeekFrom::Start(offset as u64)).unwrap();
        reader.read_exact(&mut *buffer).unwrap();

        println!("Name: {}, Offset: {:?}, Size: {:?}", name, offset, size);
        arc.index.push(BasicFile {name, size: size as usize, data: buffer});
        reader.seek(SeekFrom::Start(current_position)).unwrap();
    }
    arc
}

pub(crate) fn write_arc(mut files: Vec<BasicFile>) -> BasicFile {
    let mut writer = Cursor::new(Vec::new());
    writer.write_u32::<BigEndian>(files.len() as u32).unwrap(); // file count
    let mut entry_offsets = Vec::new();

    for i in 0..files.len() {
        write_utf8::<_, BigEndian>(&mut writer, &mut files[i].name, true);
        entry_offsets.push(writer.stream_position().unwrap());
        writer.write_u32::<BigEndian>(0).expect("Couldn't write placeholder offset"); // set offset to 0 for now
        writer.write_u32::<BigEndian>(files[i].size as u32).expect("Couldn't write file count");
    }

    for i in 0..files.len() {
        if (entry_offsets.len() != files.len()) {
            panic!("Entry offsets array is not the same length as the amount of files in the index");
        }

        // get the offset
        let offset = writer.stream_position().unwrap();

        // write the data
        writer.write(&mut files[i].data).unwrap();

        // get the location of the next entry
        let next_entry = writer.stream_position().unwrap();

        // go to correct entry
        writer.set_position(entry_offsets[i] as u64);

        // write the offset
        writer.write_u32::<BigEndian>(offset as u32).expect("Couldn't write actual offset");

        // go back to that next entry
        writer.set_position(next_entry);
    }

    stream_to_basic_file(&mut writer, "archive.arc")
}