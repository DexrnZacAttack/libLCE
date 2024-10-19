use std::io::{Read, Seek};
use std::str::from_utf8_mut;
use byteorder::{ByteOrder, ReadBytesExt};

pub(crate) fn read_utf8_prefixed_size<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> String {
    let size = reader.read_u16::<B>().unwrap();
    let mut buffer = vec![0u8; size as usize];
    reader.read_exact(&mut buffer).unwrap();
    return from_utf8_mut(&mut *buffer).expect("Couldn't read as UTF8").to_string();
}