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
mod world;
mod compression;
mod msscmp;
// I don't know basically anything about rust so it's going to be a journey

#[derive(Clone, Copy)]
pub enum ConsoleType {
    X360,
    PS3,
    WIIU,
    VITA,
    XONE,
    PS4,
    NS,
    WIN, // this exists
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::arc::Arc;
    use crate::file::types::BasicFile;
    use crate::loc::{Loc, LocIds, LocLanguage, LocLanguageId};
    use crate::msscmp::Msscmp;
    use crate::saves::Save;
    use crate::thumb::Thumbnail;
    use crate::util::image::get_raw_pixels_from_png;
    use crate::world::chunk::read_chunk;
    use byteorder::{BigEndian, ByteOrder, LittleEndian};
    use bytestream::ByteOrder::{BigEndian as OtherBigEndian, LittleEndian as OtherLittleEndian};
    use std::fs::File;
    use std::io::{BufWriter, Cursor, Read, Write};
    use std::path::Path;
    use std::time::Instant;
    use std::{fs, io};

    #[test]
    // apache2 debian default page
    fn it_works() {
        // test_chunk();
        //
        // let block_array = File::open("testblocks.bin");
        run_test(|| test_save::<BigEndian>(OtherBigEndian), "Save test - Big Endian");
        run_test(|| test_save::<LittleEndian>(OtherLittleEndian), "Save test - Little Endian");

        run_test(|| test_arc(), "ARC test");

        run_test(|| test_loc(), "LOC test");

        run_test(|| test_thumb::<BigEndian>(OtherBigEndian, true), "Thumb test - Big Endian, UTF32");
        run_test(|| test_thumb::<BigEndian>(OtherBigEndian, false), "Thumb test - Big Endian, UTF16");
        run_test(|| test_thumb::<LittleEndian>(OtherLittleEndian, true), "Thumb test - Little Endian, UTF32");
        run_test(|| test_thumb::<LittleEndian>(OtherLittleEndian, false), "Thumb test - Little Endian, UTF16");

        run_test(|| test_msscmp::<BigEndian>(OtherBigEndian, true), "MSSCMP test - Big Endian, 64-bit");
        run_test(|| test_msscmp::<BigEndian>(OtherBigEndian, false), "MSSCMP test - Big Endian, 32-bit");
        run_test(|| test_msscmp::<LittleEndian>(OtherLittleEndian, true), "MSSCMP test - Little Endian, 64-bit");
        run_test(|| test_msscmp::<LittleEndian>(OtherLittleEndian, false), "MSSCMP test - Little Endian, 32-bit");

        // let region_file = File::open("testing/test_files/region.mcr");
        // let region: Region = region::region::read_region::<_, BigEndian>(region_file.unwrap());

        // let chonk_file = File::create("testing/results/decompressed_chonk.cn").expect("Failed to create file");
        // let mut chonk_writer = BufWriter::new(chonk_file);
        // chonk_writer.write_all(&rle_decompressed.unwrap()).expect("Failed to write loc");
    }

    fn run_test<F>(func: F, desc: &str)
    where
        F: FnOnce() -> bool,
    {
        println!("\n=== {} ===", desc);
        let start = Instant::now();
        if !func() {
            println!("------ TEST FAILED (after {}.{}ms) ------", start.elapsed().as_millis(), start.elapsed().subsec_micros() % 1_000);
        } else {
            println!("------ TEST FINISHED (after {}.{}ms) ------", start.elapsed().as_millis(), start.elapsed().subsec_micros() % 1_000);
        }
    }

    fn read_all_files<P: AsRef<Path>>(folder: P) -> io::Result<Vec<BasicFile>> {
        let mut files = Vec::new();
        read_files(&folder, folder.as_ref(), &mut files)?;
        Ok(files)
    }

    fn read_files<P: AsRef<Path>>(folder: P, root: &Path, files: &mut Vec<BasicFile>) -> io::Result<()> {
        for entry in fs::read_dir(folder)? {
            let entry = entry?;
            let path = entry.path();

            if path.is_dir() {
                read_files(path, root, files)?;
            } else if path.is_file() {
                let mut file = File::open(&path)?;
                let mut data = Vec::new();
                file.read_to_end(&mut data)?;
                let basic_file = BasicFile {
                    name: path.strip_prefix(root).unwrap().to_str().unwrap_or("").to_string(),
                    size: data.len(),
                    data,
                };

                files.push(basic_file);
            }
        }
        Ok(())
    }

    fn test_region() {}

    fn test_thumb<B: ByteOrder>(byte_order: bytestream::ByteOrder, enc_utf32: bool) -> bool {
        let write_path = match (byte_order, enc_utf32) {
            (bytestream::ByteOrder::BigEndian, true) => "testing/results/thumb_big_32.dat",
            (bytestream::ByteOrder::BigEndian, false) => "testing/results/thumb_big_16.dat",
            (bytestream::ByteOrder::LittleEndian, true) => "testing/results/thumb_little_32.dat",
            (bytestream::ByteOrder::LittleEndian, false) => "testing/results/thumb_little_16.dat",
        };

        let mut write_thumb = match File::open("testing/test_files/thumb_image.png") {
            Ok(file) => Thumbnail {
                name: format!(
                    "Rust lce crate Thumb test ({}, {})",
                    match byte_order {
                        bytestream::ByteOrder::BigEndian => "Big Endian",
                        bytestream::ByteOrder::LittleEndian => "Little Endian",
                    },
                    if enc_utf32 { "UTF32" } else { "UTF16" }
                ),
                world_info: Default::default(),
                thumbnail: get_raw_pixels_from_png(file),
            },
            Err(_) => {
                eprintln!("Failed to open testing/examples/thumb_image.png. (does the file exist?)");
                return false;
            }
        };

        if Path::new(write_path).exists() {
            if let Err(_) = fs::remove_file(write_path) {
                eprintln!("Couldn't remove the old testing file");
                return false;
            }
        }

        let file = match File::create_new(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let mut writer = BufWriter::new(file);
        if let Err(_) = writer.write_all(&*write_thumb.write::<B>(if enc_utf32 { 520 } else { 256 }, enc_utf32).unwrap().data) {
            return false;
        }
        if let Err(_) = writer.flush() {
            return false;
        }

        println!("Wrote test thumb \"{}\"", write_thumb.name);

        let read_thumb = match File::open(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let thumb = Thumbnail::read::<_, B>(read_thumb, if enc_utf32 { 520 } else { 256 }, enc_utf32).unwrap();
        println!("Read thumb \"{}\"", thumb.name);
        println!("tEXt: \"{:?}\"", thumb.world_info);

        true
    }


    fn test_save<B: ByteOrder>(byte_order: bytestream::ByteOrder) -> bool {
        let write_path = match byte_order {
            bytestream::ByteOrder::BigEndian => "testing/results/test_savegame_big.dat",
            bytestream::ByteOrder::LittleEndian => "testing/results/test_savegame_little.dat",
        };

        let mut write_save = match read_all_files("testing/examples") {
            Ok(files) => Save::from(files),
            Err(_) => return false,
        };

        if Path::new(write_path).exists() {
            if let Err(_) = fs::remove_file(write_path) {
                eprintln!("Couldn't remove the old testing file");
                return false;
            }
        }

        let file = match File::create_new(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let mut writer = BufWriter::new(file);
        if let Err(_) = writer.write_all(&*write_save.write::<B>(11, 11).unwrap().data) {
            return false;
        }
        if let Err(_) = writer.flush() {
            return false;
        }

        println!("Wrote test save");

        let read_save = match File::open(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let save = Save::read::<_, B>(read_save).unwrap();
        println!("There are {} file(s) in {}", save.file_count, write_path);

        save.index.iter().for_each(|file| {
            println!("Found file \"{}\" of size {} byte(s)", file.name, file.size);
        });

        true
    }

    fn test_msscmp<B: ByteOrder>(byte_order: bytestream::ByteOrder, x64: bool) -> bool {
        let read_path = match (byte_order, x64) {
            (bytestream::ByteOrder::BigEndian, true) => "testing/test_files/msscmp_be_64.msscmp",
            (bytestream::ByteOrder::BigEndian, false) => "testing/test_files/msscmp_be_32.msscmp",
            (bytestream::ByteOrder::LittleEndian, true) => "testing/test_files/msscmp_le_64.msscmp",
            (bytestream::ByteOrder::LittleEndian, false) => "testing/test_files/msscmp_le_32.msscmp",
        };

        let read_msscmp = match File::open(read_path) {
            Ok(file) => file,
            Err(_) => {
                eprintln!("Could not find file '{}', does it exist?", read_path);
                return false;
            }
        };

        let msscmp = Msscmp::read(read_msscmp).unwrap();
        println!("Found MSSCMP name '{}'.", msscmp.name);
        println!("There are {} file(s) in {}", msscmp.files.len(), read_path);

        // this will spam the console to shit
        // msscmp.files.iter().for_each(|file| {
        //     println!("Found file \"{}\" of size {} byte(s)", file.name, file.size);
        // });

        true
    }

    fn test_arc() -> bool {
        let write_path = "testing/results/output.arc";

        let mut write_arc = match read_all_files("testing/examples") {
            Ok(files) => Arc::from(files),
            Err(_) => return false,
        };

        if Path::new(write_path).exists() {
            if let Err(_) = fs::remove_file(write_path) {
                eprintln!("Couldn't remove the old testing file");
                return false;
            }
        }

        let file = match File::create_new(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let mut writer = BufWriter::new(file);
        if let Err(_) = writer.write_all(&*write_arc.write().unwrap().data) {
            return false;
        }
        if let Err(_) = writer.flush() {
            return false;
        }

        println!("Wrote test arc");

        let read_arc = match File::open(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let arc = Arc::read(read_arc).unwrap();
        println!("There are {} file(s) in {}", arc.file_count, write_path);

        arc.index.iter().for_each(|file| {
            println!("Found file \"{}\" of size {} byte(s)", file.name, file.size);
        });

        true
    }

    fn test_loc() -> bool {
        let write_path = "testing/results/output.loc";

        let write_loc = Loc {
            version: 2,
            language_count: 2,
            keys: LocIds {
                use_unique_ids: true,
                count: 1,
                ids: vec![0],
            },
            language_ids: vec![LocLanguageId { language_id: "en-EN".parse().unwrap(), unknown: 0 }, LocLanguageId { language_id: "zh-CN".parse().unwrap(), unknown: 0 }],
            languages: vec![LocLanguage { should_read_byte: 1, byte: 0, language_name: "en-EN".parse().unwrap(), string_count: 1, strings: vec!["Hello!".to_string()] }, LocLanguage { should_read_byte: 1, byte: 0, language_name: "zh-CN".parse().unwrap(), string_count: 1, strings: vec!["你好！".to_string()] }],
        };


        if Path::new(write_path).exists() {
            if let Err(_) = fs::remove_file(write_path) {
                eprintln!("Couldn't remove the old testing file");
                return false;
            }
        }

        let file = match File::create_new(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let mut writer = BufWriter::new(file);
        if let Err(_) = writer.write_all(&*write_loc.write().unwrap().data) {
            return false;
        }
        if let Err(_) = writer.flush() {
            return false;
        }

        println!("Wrote test loc");

        let read_loc = match File::open(write_path) {
            Ok(file) => file,
            Err(_) => return false,
        };
        let loc = Loc::read(read_loc).unwrap();
        println!("There are {} language(s) in {}", loc.languages.len(), write_path);

        loc.language_ids.iter().for_each(|lang| {
            println!("Found language \"{}\"", lang.language_id);
        });

        loc.languages.iter().for_each(|lang| {
            println!("There are {} string(s) in language \"{}\"", lang.string_count, lang.language_name);
        });

        true
    }

    // NOT IN CURRENT BUILD
    // fn test_chunk() {
    //     let chunk_file = File::open("testing/test_files/chunk.mcr");
    //     let mut chunk = world::chunk::read_console_compressed_chunk::<_, BigEndian>(chunk_file.unwrap(), ConsoleType::PS3);
    //     let c_decompressed = world::chunk::decompress_console_compressed_chunk(&mut chunk.chunk, ConsoleType::PS3);
    //     let rle_decompressed = compression::chunk::decompress_chunk(&mut c_decompressed.unwrap());
    // 
    //     println!("{:?}", read_chunk::<_, BigEndian>(Cursor::new(rle_decompressed)));
    // }
}
