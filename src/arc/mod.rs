/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use crate::file::types::BasicFile;
use crate::util::io::ex_string::{ExStringR, ExStringW};
use crate::util::io::rw::stream_to_basic_file;
use byteorder::{BigEndian, ReadBytesExt, WriteBytesExt};
use std::io::{Cursor, Error, ErrorKind, Read, Seek, SeekFrom, Write};

/// Archive files, used for storing game assets. (usually under Media{platform}.arc)
#[derive(Default, Debug)]
pub struct Arc {
    pub file_count: u32,
    pub index: Vec<BasicFile>,
}

impl Arc {
    /// Reads a .arc file and returns an Arc instance.
    pub fn read<R: Read + Seek + std::fmt::Debug>(mut reader: R) -> Result<Arc, std::io::Error> {
        let mut arc = Arc::default();
        let index_count = reader.read_u32::<BigEndian>()?;
        arc.file_count = index_count;

        for _ in 0..index_count {
            let name = reader.read_utf8_prefixed_size::<BigEndian>();
            let offset = reader.read_u32::<BigEndian>()?;
            let size = reader.read_u32::<BigEndian>()?;
            let current_position = reader.stream_position()?;

            let mut buffer = vec![0u8; size as usize];
            reader.seek(SeekFrom::Start(offset as u64))?;
            reader.read_exact(&mut *buffer)?;

            arc.index.push(BasicFile { name, size: size as usize, data: buffer });
            reader.seek(SeekFrom::Start(current_position))?;
        }
        Ok(arc)
    }

    /// Returns a BasicFile from an Arc which can be written to disk
    pub fn write(&mut self) -> Result<BasicFile, Error> {
        let mut writer = Cursor::new(Vec::new());
        let files = &mut self.index;
        writer.write_u32::<BigEndian>(files.len() as u32)?; // file count
        let mut entry_offsets = Vec::new();

        for i in 0..files.len() {
            writer.write_utf8::<BigEndian>(&mut files[i].name, true);
            entry_offsets.push(writer.stream_position()?);
            writer.write_u32::<BigEndian>(0)?; // set offset to 0 for now
            writer.write_u32::<BigEndian>(files[i].size as u32)?;
        }

        for i in 0..files.len() {
            if entry_offsets.len() != files.len() {
                return Err(Error::new(ErrorKind::InvalidData, "Entry offsets array is not the same length as the amount of files in the index"));
            }

            // get the offset
            let offset = writer.stream_position()?;

            // write the data
            writer.write(&mut files[i].data)?;

            // get the location of the next entry
            let next_entry = writer.stream_position()?;

            // go to correct entry
            writer.set_position(entry_offsets[i] as u64);

            // write the offset
            writer.write_u32::<BigEndian>(offset as u32)?;

            // go back to that next entry
            writer.set_position(next_entry);
        }

        Ok(stream_to_basic_file(&mut writer, "archive.arc"))
    }
}

impl From<Vec<BasicFile>> for Arc {
    fn from(value: Vec<BasicFile>) -> Self {
        let mut arc = Arc::default();

        value.iter().for_each(|file| {
            arc.file_count += 1;
            arc.index.push(file.clone());
        });

        arc
    }
}