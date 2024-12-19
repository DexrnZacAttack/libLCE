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
use serde::Serialize;
use std::io::{Cursor, Error, Read, Seek};

/// Loc Language
///
/// Contains the language name and strings.
///
/// There is also an unknown `u8` which is only read if the preceding `u32` is larger than 0.
#[derive(Default, Debug, Serialize)]
pub struct LocLanguage {
    pub should_read_byte: u32,
    pub byte: u8,
    pub language_name: String,
    pub string_count: u32,
    pub strings: Vec<String>,
}

/// Loc String IDs
///
/// Seems to be used for getting a string based on the ID, how it does this is unknown (maybe a hash?)
#[derive(Default, Debug, Serialize)]
pub struct LocIds {
    pub use_unique_ids: bool,
    pub count: u32,
    pub ids: Vec<u32>,
}

/// Loc Language ID
///
/// It is not known what `unknown` currently does (hence the name)
#[derive(Default, Debug, Serialize)]
pub struct LocLanguageId {
    /// Language ID (e.g `en-EN`)
    pub language_id: String,
    /// Probably some language key? Not sure.
    pub unknown: u32,
}

/// Localization file
///
/// Holds translation strings for several different languages.
///
/// **NOTE: This implementation is not 100% perfect.**
#[derive(Default, Debug, Serialize)]
pub struct Loc {
    pub version: u32,
    pub language_count: u32,
    pub keys: LocIds,
    pub language_ids: Vec<LocLanguageId>,
    pub languages: Vec<LocLanguage>,
}

impl Loc {
    /// Reads a .loc file and returns a Loc instance.
    pub fn read<R: Read + Seek + std::fmt::Debug>(mut reader: R) -> Result<Loc, Error> {
        let mut loc = Loc::default();
        loc.version = reader.read_u32::<BigEndian>()?;
        loc.language_count = reader.read_u32::<BigEndian>()?;

        if loc.version == 2 {
            loc.keys.use_unique_ids = reader.read_u8()? == 1;
            loc.keys.count = reader.read_u32::<BigEndian>()?;
            for _ in 0..loc.keys.count {
                loc.keys.ids.push(reader.read_u32::<BigEndian>()?);
            }
        }

        for _ in 0..loc.language_count {
            let lang_id = LocLanguageId { language_id: reader.read_utf8_prefixed_size::<BigEndian>(), unknown: reader.read_u32::<BigEndian>()? };
            loc.language_ids.push(lang_id)
        }

        for _ in 0..loc.language_count {
            let mut language = LocLanguage::default();
            language.should_read_byte = reader.read_u32::<BigEndian>()?;
            if language.should_read_byte > 0 {
                language.byte = reader.read_u8()?;
            }
            language.language_name = reader.read_utf8_prefixed_size::<BigEndian>();
            language.string_count = reader.read_u32::<BigEndian>()?;
            language.strings = vec![];
            for _ in 0..language.string_count {
                language.strings.push(reader.read_utf8_prefixed_size::<BigEndian>());
            }
            loc.languages.push(language);
        }

        Ok(loc)
    }

    /// Returns a BasicFile from a Loc which can be written to disk
    pub fn write(&self) -> Result<BasicFile, Error> {
        let mut writer = Cursor::new(Vec::new());

        writer.write_u32::<BigEndian>(self.version)?;
        writer.write_u32::<BigEndian>(self.language_count)?;

        if self.version == 2 {
            writer.write_u8(self.keys.use_unique_ids as u8)?;
            writer.write_u32::<BigEndian>(self.keys.count)?;
            for &key in &self.keys.ids {
                writer.write_u32::<BigEndian>(key)?;
            }
        }

        for id in 0..self.language_count {
            writer.write_utf8::<BigEndian>(&*self.language_ids[id as usize].language_id, true);
            writer.write_u32::<BigEndian>(self.language_ids[id as usize].unknown)?;
        }

        for lang in 0..self.language_count {
            writer.write_u32::<BigEndian>(self.languages[lang as usize].should_read_byte)?;
            writer.write_u8(self.languages[lang as usize].byte)?;
            writer.write_utf8::<BigEndian>(&*self.languages[lang as usize].language_name, true);
            writer.write_u32::<BigEndian>(self.languages[lang as usize].string_count)?;
            for string in 0..self.languages[lang as usize].string_count {
                writer.write_utf8::<BigEndian>(&*self.languages[lang as usize].strings[string as usize], true);
            }
        }

        Ok(stream_to_basic_file(&mut writer, "languages.loc"))
    }
}