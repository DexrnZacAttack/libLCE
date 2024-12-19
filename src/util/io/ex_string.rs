/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use crate::util;
use byteorder::{ByteOrder, ReadBytesExt, WriteBytesExt};
use std::io::{Read, Seek, SeekFrom, Write};
use std::str::from_utf8_mut;
use utf16_ext::Utf16WriteExt;

// result of NIH? probably.

pub(crate) trait ExStringR {
    fn read_utf8(&mut self, size: usize) -> String;
    fn read_utf8_prefixed_size<B: ByteOrder>(&mut self) -> String;
    fn read_utf8_null_terminated(&mut self) -> String;
    fn read_utf16_prefixed_size<B: ByteOrder>(&mut self) -> String;
    fn read_utf16<B: ByteOrder>(&mut self, size: usize) -> String;
    fn read_utf32<B: ByteOrder>(&mut self, size: usize) -> String; // thanks nintendo switch edition (or rather Unix-based systems choosing to use 32-bit wchar) 
    fn read_utf16_null_terminated<B: ByteOrder>(&mut self) -> String;
    fn read_utf32_null_terminated<B: ByteOrder>(&mut self) -> String;
}

impl<R: Read + Seek> ExStringR for R {
    fn read_utf8(&mut self, size: usize) -> String {
        let mut buffer = vec![0u8; size];
        self.read_exact(&mut buffer).expect("Couldn't read into the buffer.");
        from_utf8_mut(&mut *buffer).expect("Couldn't read as UTF8").to_string()
    }

    fn read_utf8_prefixed_size<B: ByteOrder>(&mut self) -> String {
        let size = self.read_u16::<B>().expect("Couldn't read the string size. (of size u16)");
        self.read_utf8(size as usize)
    }

    fn read_utf8_null_terminated(&mut self) -> String {
        let pos = self.stream_position();
        let mut size = 0;

        while self.read_u8().unwrap() != 0 {
            size += 1;
        }

        self.seek(SeekFrom::Start(pos.unwrap())).unwrap();
        let mut buffer = vec![0u8; size];
        self.read_exact(&mut buffer).expect("Couldn't read into the buffer.");
        from_utf8_mut(&mut *buffer).expect("Couldn't read as UTF8").to_string()
    }

    fn read_utf16_prefixed_size<B: ByteOrder>(&mut self) -> String {
        let size = self.read_u16::<B>().unwrap();
        self.read_utf16::<B>(size as usize)
    }

    fn read_utf16<B: ByteOrder>(&mut self, size: usize) -> String {
        let mut buffer = vec![0u8; size];
        self.read_exact(&mut buffer).expect("Couldn't read into the buffer.");
        let u16_string = util::array::u8_to_u16_vec::<B>(buffer).unwrap();
        String::from_utf16(&u16_string).expect("Couldn't create a valid UTF16 string from the characters in the buffer. (invalid string?)")
    }

    // thanks nintendo switch edition
    fn read_utf32<B: ByteOrder>(&mut self, size: usize) -> String {
        let mut buffer = vec![0u8; size * 4];
        self.read_exact(&mut buffer).expect("Couldn't read into the buffer.");
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

        // IDK how this ok stuff works lmfao
        chars.into_iter().collect()
    }

    fn read_utf16_null_terminated<B: ByteOrder>(&mut self) -> String {
        let mut buffer = Vec::new();
        let mut char = [0u8; 2];

        loop {
            match self.read_exact(&mut char) {
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

    fn read_utf32_null_terminated<B: ByteOrder>(&mut self) -> String {
        let mut buffer = Vec::new();
        let mut char = [0u8; 4];

        loop {
            match self.read_exact(&mut char) {
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
}

pub(crate) trait ExStringW {
    fn write_utf8<B: ByteOrder>(&mut self, string: &str, write_length: bool);
    fn write_utf32<B: ByteOrder>(&mut self, string: &str, write_length: bool);
    fn write_utf32_padded<B: ByteOrder>(&mut self, string: &str, padding_size: u16, subtract_from_length: bool, write_length: bool);
    fn write_utf16<B: ByteOrder>(&mut self, string: &str, write_length: bool);
    fn write_utf16_padded<B: ByteOrder>(&mut self, string: &str, padding_size: u16, subtract_from_length: bool, write_length: bool);
}

impl<T: Write> ExStringW for T {
    fn write_utf8<B: ByteOrder>(&mut self, string: &str, write_length: bool) {
        if write_length && string.len() > 65535 {
            panic!("String size is too large!");
        }

        if write_length {
            self.write_u16::<B>(string.len() as u16).unwrap();
        }
        self.write_all(string.as_ref()).unwrap();
    }

    fn write_utf32<B: ByteOrder>(&mut self, string: &str, write_length: bool) {
        if write_length && string.len() > 65535 {
            panic!("String size is too large!");
        }

        if write_length {
            self.write_u16::<B>(string.len() as u16);
        }
        for character in string.chars() {
            self.write_u32::<B>(character as u32);
        }
    }

    fn write_utf32_padded<B: ByteOrder>(&mut self, string: &str, mut padding_size: u16, subtract_from_length: bool, write_length: bool) {
        if write_length && string.len() > 65535 {
            panic!("String size is too large!");
        }

        let utf32_length = string.chars().count() as u16;
        if subtract_from_length && (padding_size < utf32_length) {
            panic!("String size is larger than the padding!");
        }

        if write_length {
            self.write_u16::<B>(utf32_length).unwrap();
        }

        for character in string.chars() {
            self.write_u32::<B>(character as u32).unwrap();
        }

        if subtract_from_length {
            padding_size -= utf32_length;
        }

        for _ in 0..padding_size {
            self.write_u32::<B>(0).unwrap();
        }
    }

    fn write_utf16<B: ByteOrder>(&mut self, string: &str, write_length: bool) {
        if write_length && string.len() > 65535 {
            panic!("String size is too large!");
        }

        if write_length {
            self.write_u16::<B>(string.len() as u16).unwrap();
        }
        self.write_utf16_string::<B>(string).unwrap();
    }

    fn write_utf16_padded<B: ByteOrder>(&mut self, string: &str, mut padding_size: u16, subtract_from_length: bool, write_length: bool) {
        if write_length && string.len() > 65535 {
            panic!("String size is too large!");
        }

        if subtract_from_length && (padding_size < (string.encode_utf16().count() as u16)) {
            panic!("String size is larger than the padding!");
        }

        if write_length {
            self.write_u16::<B>(string.len() as u16).unwrap();
        }
        self.write_utf16_string::<B>(string).unwrap();

        if subtract_from_length {
            padding_size = padding_size - (string.encode_utf16().count() as u16);
        }

        for _ in 0..padding_size {
            self.write_u16::<B>(0).unwrap();
        }
    }
}