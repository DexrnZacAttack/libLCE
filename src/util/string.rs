use std::io::{Read, Seek};
use std::str::from_utf8_mut;
use byteorder::{ByteOrder, ReadBytesExt};
use crate::util;

pub(crate) fn read_utf8_prefixed_size<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> String {
    let size = reader.read_u16::<B>().unwrap();
    let mut buffer = vec![0u8; size as usize];
    reader.read_exact(&mut buffer).unwrap();
    return from_utf8_mut(&mut *buffer).expect("Couldn't read as UTF8").to_string();
}

pub(crate) fn read_utf8<R: Read + Seek + std::fmt::Debug>(mut reader: R, size: u128) -> String {
    let mut buffer = vec![0u8; size as usize];
    reader.read_exact(&mut buffer).unwrap();
    return from_utf8_mut(&mut *buffer).expect("Couldn't read as UTF8").to_string();
}

pub(crate) fn read_utf16_prefixed_size<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> String {
    let size = reader.read_u16::<B>().unwrap();
    let mut buffer = vec![0u8; size as usize];
    reader.read_exact(&mut buffer).unwrap();
    let u16_string = util::array::u8_to_u16_array::<B>(&buffer).unwrap();
    let utf16_string = String::from_utf16(&u16_string).unwrap();
    utf16_string.to_string()
}

pub(crate) fn read_utf16<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R, size: u128) -> String {
    let mut buffer = vec![0u8; size as usize];
    reader.read_exact(&mut buffer).unwrap();
    let u16_string = util::array::u8_to_u16_array::<B>(&buffer).unwrap();
    let utf16_string = String::from_utf16(&u16_string).unwrap();
    utf16_string.to_string()
}