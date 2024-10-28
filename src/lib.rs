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
mod thumb;
mod loc;
// I don't know basically anything about rust so it's gonna be a journey

#[cfg(test)]
mod tests {
    use std::collections::HashMap;
    use std::fs;
    use std::fs::File;
    use std::io::{BufWriter, Write};
    use byteorder::{BigEndian, LittleEndian};
    use serde_json::to_writer;
    use crate::saves::SaveCompression;
    use walkdir::WalkDir;
    use crate::loc::Loc;
    use crate::thumb::{FjText, Thumbnail};
    use crate::util::image::get_raw_pixels_from_png;
    use super::*;

    #[test]
    // apache2 debian default page
    fn it_works() {
        let mut test = Vec::new();
        let arc_file = File::open("testing/testfiles/media.arc");
        let arc = arc::read_arc::<_>(arc_file.unwrap());
        let save_file = File::open("testing/testfiles/savegame.dat");
        let save = saves::read_save::<_, BigEndian>(save_file.unwrap());

        let thumb_file = File::open("testing/testfiles/THUMB_NS");
        let thumbnail: Thumbnail = thumb::read_thumb::<_, LittleEndian>(thumb_file.unwrap(), 520, true);

        println!("{:?}", thumbnail);

        let loc_file = File::open("testing/testfiles/languages.loc");
        let loc: Loc = loc::read_loc(loc_file.unwrap());

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

        let write_thumb = thumb::write_thumb::<BigEndian>(Thumbnail {
            name: "crust test".to_string(),
            world_info: FjText {
                fj_seed: "123123123123123123123123123123123123123".to_string(),
                fj_base_save_name: "does this even do anything normally".to_string(),
                fj_host_options: "fffffff".to_string(),
                ..Default::default()
            },
            thumbnail: get_raw_pixels_from_png(File::open("testing/testfiles/example_thumb_trollface.png").unwrap())
        }, 256, false);
        let thumb_file = File::create("testing/results/THUMB.ext").expect("Failed to create file");
        let mut thumb_writer = BufWriter::new(thumb_file);
        thumb_writer.write_all(&write_thumb.data).expect("Failed to write thumb");

        let loc_json = File::create("testing/results/locTest.json").expect("Failed to create file");
        let loc_json_writer = BufWriter::new(loc_json);
        to_writer(loc_json_writer, &loc);

        let write_loc = loc::write_loc(loc);
        let loc_file = File::create("testing/results/locTest.loc").expect("Failed to create file");
        let mut loc_writer = BufWriter::new(loc_file);
        loc_writer.write_all(&write_loc.data).expect("Failed to write loc");
    }
}
