/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Read, Seek, Write};
use std::str::from_utf8_mut;
use byteorder::{ByteOrder, ReadBytesExt, WriteBytesExt};
use utf16_ext::Utf16WriteExt;
use crate::util;

pub(crate) fn read_utf8_prefixed_size<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> String {
    let size = reader.read_u16::<B>().unwrap();
    read_utf8(&mut reader, size as usize)
}

pub(crate) fn read_utf8<R: Read + Seek + std::fmt::Debug>(mut reader: R, size: usize) -> String {
    let mut buffer = vec![0u8; size];
    reader.read_exact(&mut buffer).unwrap();
    from_utf8_mut(&mut *buffer).expect("Couldn't read as UTF8").to_string()
}

pub(crate) fn read_utf16_prefixed_size<R: Read + Seek + std::fmt::Debug, B: ByteOrder>(mut reader: R) -> String {
    let size = reader.read_u16::<B>().unwrap();
    read_utf16::<_, B>(&mut reader, size as usize)
}

pub(crate) fn read_utf16<R: Read, B: ByteOrder>(mut reader: R, size: usize) -> String {
    let mut buffer = vec![0u8; size];
    reader.read_exact(&mut buffer).unwrap();
    let u16_string = util::array::u8_to_u16_vec::<B>(buffer).unwrap();
    String::from_utf16(&u16_string).unwrap()
}

pub(crate) fn write_utf8<W: Write, B: ByteOrder>(mut writer: W, string: &str, write_length: bool) {
    if (write_length && string.len() > 65535) {
        panic!("String size is too large!");
    }

    if (write_length) {
        writer.write_u16::<B>(string.len() as u16).unwrap();
    }
    writer.write_all(string.as_ref()).unwrap();
}

pub(crate) fn write_utf16<W: Write, B: ByteOrder>(mut writer: W, string: &str, write_length: bool) {
    if (write_length && string.len() > 65535) {
        panic!("String size is too large!");
    }

    if (write_length) {
        writer.write_u16::<B>(string.len() as u16).unwrap();
    }
    writer.write_utf16_string::<B>(string).unwrap();
}

pub(crate) fn write_utf16_padded<W: Write, B: ByteOrder>(mut writer: W, string: &str, mut padding_size: u16, subtract_from_length: bool, write_length: bool) {
    if (write_length && string.len() > 65535) {
        panic!("String size is too large!");
    }

    if (subtract_from_length && (padding_size < (string.encode_utf16().count() as u16))) {
        panic!("String size is larger than the padding!");
    }
    
    if (write_length) {
        writer.write_u16::<B>(string.len() as u16).unwrap();
    }
    writer.write_utf16_string::<B>(string).unwrap();

    if subtract_from_length {
        padding_size = padding_size - (string.encode_utf16().count() as u16);
    }

    for i in 0..padding_size {
        writer.write_u16::<B>(0).unwrap();
    }
}