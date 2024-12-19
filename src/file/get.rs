/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

use crate::file::types::BasicFile;
use std::fs::read;
use std::path::PathBuf;

pub(crate) fn get_basic_file_from_path(path_buf: PathBuf, override_name: &str) -> BasicFile {
    let mut name = path_buf.file_name()
        .and_then(|name| name.to_str())
        .unwrap_or_default()
        .to_string();

    if !override_name.is_empty() {
        name = override_name.parse().unwrap();
    }

    let file = read(path_buf).unwrap();
    BasicFile { name: name.to_string(), size: file.len(), data: file }
}