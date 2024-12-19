use crate::util;
use crate::util::io::ex_string::ExStringR;
use bytestream::ByteOrder::LittleEndian;
use bytestream::{ByteOrder, StreamReader};
use std::io::{Error, Read, Seek, SeekFrom};

/// Binka file found within an MSSCMP
#[derive(Default, Debug)]
pub struct MsscmpFile {
    pub name: String,
    pub size: u64,
    pub sample_rate: u32,
    pub data: Vec<u8>,
}

/// Miles Sound System Compressed Archive
pub struct Msscmp {
    pub name: String,
    pub files: Vec<MsscmpFile>,
}

impl Msscmp {
    /// Reads an MSSCMP file and returns an Msscmp instance.
    pub fn read<R: Read + Seek + std::fmt::Debug>(mut reader: R) -> Result<Msscmp, Error> {
        let mut files = vec![MsscmpFile::default()];
        let magic = reader.read_utf8(4);
        let mut new_gen = false;

        let byte_order: ByteOrder = match magic.as_str() {
            "KNAB" => LittleEndian,
            _ => ByteOrder::BigEndian
        };

        println!("{}", magic);

        reader.seek(SeekFrom::Start(0x18))?;
        if u32::read_from(&mut reader, byte_order)? != u32::read_from(&mut reader, byte_order)? {
            println!("New-gen MSSCMP");
            new_gen = true;
        } else {
            println!("Old-gen MSSCMP");
        }

        reader.seek(SeekFrom::Start(0x04))?;

        // I forgot what this is
        u32::read_from(&mut reader, byte_order)?;

        let _data_start_offset = util::io::rw::read_c_ulong(&mut reader, new_gen, byte_order);

        u64::read_from(&mut reader, byte_order)?;

        // Offset of the first index
        let _first_index_offset = util::io::rw::read_c_ulong(&mut reader, new_gen, byte_order);

        // Marks the last offset in the first index
        let last_entry_offset = util::io::rw::read_c_ulong(&mut reader, new_gen, byte_order);

        u64::read_from(&mut reader, byte_order)?;
        if new_gen {
            u64::read_from(&mut reader, byte_order)?;
        }

        // seems to always have 0x03000000 at the start (le)
        let _unknown_offset = util::io::rw::read_c_ulong(&mut reader, new_gen, byte_order);

        // How many entries are in index 1
        let _index_1_count = util::io::rw::read_c_ulong(&mut reader, new_gen, byte_order);

        util::io::rw::read_c_ulong(&mut reader, new_gen == false, byte_order);

        // How many entries are in index 2
        let count = u32::read_from(&mut reader, byte_order)?;

        // MSSCMP root name
        let name = reader.read_utf8(12);

        println!("Name: {}", name);

        reader.seek(SeekFrom::Start(last_entry_offset + 4))?;

        for i in 0..count {
            reader.seek(SeekFrom::Start((last_entry_offset + 4) + (i * 8) as u64))?;

            let entry_offset = u32::read_from(&mut reader, byte_order)?;
            let mut _f_structure_offset = 0;
            if i != count {
                _f_structure_offset = u32::read_from(&mut reader, byte_order)?;
            }

            reader.seek(SeekFrom::Start(entry_offset as u64))?;

            let name_offset = u32::read_from(&mut reader, byte_order)?;
            let cur_offset = reader.stream_position();

            reader.seek(SeekFrom::Start(name_offset as u64))?;
            let file_name = reader.read_utf8_null_terminated();

            reader.seek(SeekFrom::Start(cur_offset?))?;

            u32::read_from(&mut reader, byte_order)?;

            let data_offset = u32::read_from(&mut reader, LittleEndian)?;
            u32::read_from(&mut reader, byte_order)?;
            u32::read_from(&mut reader, byte_order)?;
            let sample_rate = u32::read_from(&mut reader, byte_order)?;
            let file_size = u32::read_from(&mut reader, byte_order)?;

            reader.seek(SeekFrom::Start(data_offset as u64))?;

            let mut data: Vec<u8> = vec![0; file_size as usize];

            reader.read_exact(&mut data)?;

            files.push(MsscmpFile {
                name: file_name,
                size: file_size as u64,
                sample_rate,
                data,
            })
        }

        Ok(Msscmp { name, files })
    }
}
