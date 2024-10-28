/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use std::io::{Cursor, Read, Seek};
use byteorder::{BigEndian, ReadBytesExt, WriteBytesExt};
use serde::Serialize;
use crate::file::types::BasicFile;
use crate::util::io::rw::stream_to_basic_file;
use crate::util::io::string::{read_utf8_prefixed_size, write_utf8};

#[derive(Default, Debug, Serialize)]
pub struct LocLanguage {
    should_read_byte: u32,
    byte: u8,
    language_name: String,
    string_count: u32,
    strings: Vec<String>
}

#[derive(Default, Debug, Serialize)]
pub struct LocKeys {
    use_unique_ids: bool,
    count: u32,
    keys: Vec<u32>
}

#[derive(Default, Debug, Serialize)]
pub struct LocLanguageId {
    language_id: String,
    unknown: u32
}

#[derive(Default, Debug, Serialize)]
pub struct Loc {
    version: u32,
    count: u32,
    keys: LocKeys,
    language_ids: Vec<LocLanguageId>,
    languages: Vec<LocLanguage>
}

pub fn read_loc<R: Read + Seek + std::fmt::Debug>(mut reader: R) -> Loc {
    let mut Loc = Loc::default();
    Loc.version = reader.read_u32::<BigEndian>().unwrap();
    Loc.count = reader.read_u32::<BigEndian>().unwrap();

    if (Loc.version == 2) {
        Loc.keys.use_unique_ids = reader.read_u8().unwrap() == 1;
        Loc.keys.count = reader.read_u32::<BigEndian>().unwrap();
        for _ in 0..Loc.keys.count {
            Loc.keys.keys.push(reader.read_u32::<BigEndian>().unwrap());
        }
    }

    for _ in 0..Loc.count {
        let lang_id = LocLanguageId {language_id: read_utf8_prefixed_size::<_, BigEndian>(&mut reader), unknown: reader.read_u32::<BigEndian>().unwrap()};
        Loc.language_ids.push(lang_id)
    }

    for _ in 0..Loc.count {
        let mut language = LocLanguage::default();
        language.should_read_byte = reader.read_u32::<BigEndian>().unwrap();
        if (language.should_read_byte > 1) {
            language.byte = reader.read_u8().unwrap();
        }
        language.language_name = read_utf8_prefixed_size::<_, BigEndian>(&mut reader);
        language.string_count = reader.read_u32::<BigEndian>().unwrap();
        language.strings = vec![];
        for _ in 0..language.string_count {
            language.strings.push(read_utf8_prefixed_size::<_, BigEndian>(&mut reader));
        }
        Loc.languages.push(language);
    }

    Loc
}

pub fn write_loc(loc: Loc) -> BasicFile {
    let mut writer = Cursor::new(Vec::new());

    writer.write_u32::<BigEndian>(loc.version).unwrap();
    writer.write_u32::<BigEndian>(loc.count).unwrap();

    if (loc.version == 2) {
        writer.write_u8(loc.keys.use_unique_ids as u8);
        writer.write_u32::<BigEndian>(loc.keys.count).unwrap();
        for key in loc.keys.keys {
            writer.write_u32::<BigEndian>(key).unwrap();
        }
    }

    for id in 0..loc.count {
        write_utf8::<_, BigEndian>(&mut writer, &*loc.language_ids[id as usize].language_id, true);
        writer.write_u32::<BigEndian>(loc.language_ids[id as usize].unknown).unwrap();
    }

    for lang in 0..loc.count {
        writer.write_u32::<BigEndian>(loc.languages[lang as usize].should_read_byte).unwrap();
        writer.write_u8(loc.languages[lang as usize].byte).unwrap();
        write_utf8::<_, BigEndian>(&mut writer, &*loc.languages[lang as usize].language_name, true);
        writer.write_u32::<BigEndian>(loc.languages[lang as usize].string_count);
        for string in 0..loc.languages[lang as usize].string_count {
            write_utf8::<_, BigEndian>(&mut writer, &*loc.languages[lang as usize].strings[string as usize], true);
        }
    }

    stream_to_basic_file(&mut writer, "languages.loc")
}