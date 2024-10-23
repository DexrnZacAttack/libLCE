/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/
mod saves;
mod arc;
mod util;
mod file;
// I don't know basically anything about rust so it's gonna be a journey

#[cfg(test)]
mod tests {
    use std::fs;
    use std::fs::File;
    use std::io::{BufWriter, Write};
    use byteorder::BigEndian;
    use crate::saves::SaveCompression;
    use walkdir::WalkDir;
    use super::*;

    #[test]
    // apache2 debian default page
    fn it_works() {
        let mut test = Vec::new();
        let arc_file = File::open("testing/testfiles/media.arc");
        let arc = arc::read_arc::<_>(arc_file.unwrap());
        let save_file = File::open("testing/testfiles/savegame.dat");
        let save = saves::read_save::<_, BigEndian>(save_file.unwrap());

        for entry in WalkDir::new("testing/save_test").into_iter().filter_map(Result::ok) {
            let path = entry.path();
            if path.is_file() {
                if let Ok(rel_path) = path.strip_prefix("testing/save_test") {
                    test.push(file::get::get_basic_file_from_path(path.to_path_buf(), &*rel_path.to_path_buf().to_str().unwrap().replace("\\", "/")));
                }
            }
        }

        let writeArc = arc::write_arc(test.clone());
        let file = File::create("testing/results/output.arc").expect("Failed to create file");
        let mut writer = BufWriter::new(file);
        writer.write_all(&writeArc.data).expect("Failed to write ARC");

        let write_save = saves::compress_save::<BigEndian>(saves::write_save::<BigEndian>(test, 1, 1), SaveCompression::ZLIB);
        let save_file = File::create("testing/results/savegame.dat").expect("Failed to create file");
        let mut save_writer = BufWriter::new(save_file);
        save_writer.write_all(&write_save.data).expect("Failed to write save");
    }
}
