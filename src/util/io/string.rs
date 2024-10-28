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

// thanks nintendo switch edition
pub(crate) fn read_utf32<R: Read, B: ByteOrder>(mut reader: R, size: usize) -> Result<String, std::io::Error> {
    let mut buffer = vec![0u8; size * 4]; 
    reader.read_exact(&mut buffer)?;
    let u32_string: Vec<u32> = (0..size)
        .map(|i| B::read_u32(&buffer[i * 4..]))
        .collect();

    let mut chars: Vec<char> = Vec::new();
    for codepoint in u32_string {
        if let Some(c) = std::char::from_u32(codepoint) {
            chars.push(c);
        } else {
            chars.push('?')
        }
    }

    // idk how this ok stuff works lmfao
    Ok(chars.into_iter().collect())
}

pub(crate) fn read_utf16_null_terminated<R: Read, B: ByteOrder>(mut reader: R) -> String {
    let mut buffer = Vec::new();
    let mut char = [0u8; 2];

    loop {
        match reader.read_exact(&mut char) {
            Ok(()) => {
                if char == [0, 0] {
                    break; 
                }
                buffer.extend_from_slice(&char); 
            }
            Err(_) => {
                break;
            }
        }
    }

    let u16_string = util::array::u8_to_u16_vec::<B>(buffer).unwrap();
    String::from_utf16(&u16_string).unwrap()
}

pub(crate) fn read_utf32_null_terminated<R: Read, B: ByteOrder>(mut reader: R) -> String {
    let mut buffer = Vec::new();
    let mut char = [0u8; 4];

    loop {
        match reader.read_exact(&mut char) {
            Ok(()) => {
                if char == [0, 0, 0, 0] {
                    break;
                }
                buffer.extend_from_slice(&char);
            }
            Err(_) => {
                break;
            }
        }
    }

    let u32_string: Vec<u32> = buffer.chunks_exact(4)
        .map(|chunk| B::read_u32(chunk))
        .collect();

    let mut str = String::new();
    for codepoint in u32_string {
        if let Some(c) = char::from_u32(codepoint) {
            str.push(c);
        } else {
            str.push('?');
        }
    }

    str
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

pub(crate) fn write_utf32<W: Write, B: ByteOrder>(mut writer: W, string: &str, write_length: bool) {
    if write_length && string.len() > 65535 {
        panic!("String size is too large!");
    }

    if write_length {
        writer.write_u16::<B>(string.len() as u16);
    }
    for character in string.chars() {
        writer.write_u32::<B>(character as u32);
    }
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

pub(crate) fn write_utf32_padded<W: Write, B: ByteOrder>(mut writer: W, string: &str, mut padding_size: u16, subtract_from_length: bool, write_length: bool) {
    if write_length && string.len() > 65535 {
        panic!("String size is too large!");
    }

    let utf32_length = string.chars().count() as u16;
    if subtract_from_length && (padding_size < utf32_length) {
        panic!("String size is larger than the padding!");
    }

    if write_length {
        writer.write_u16::<B>(utf32_length).unwrap();
    }

    for character in string.chars() {
        writer.write_u32::<B>(character as u32).unwrap();
    }

    if subtract_from_length {
        padding_size -= utf32_length;
    }

    for _ in 0..padding_size {
        writer.write_u32::<B>(0).unwrap(); 
    }
}