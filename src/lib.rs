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

// I don't know basically anything about rust so it's gonna be a journey

#[derive(Default, Debug)]
pub struct BasicFile {
    name: String,
    size: u32,
    offset: u32,
    data: Vec<u8>
}

#[cfg(test)]
mod tests {
    use std::fs::File;
    use byteorder::BigEndian;
    use super::*;

    #[test]
    // apache2 debian default page
    fn it_works() {
        let arc_file = File::open("testing/media.arc");
        let arc = arc::read_arc::<_>(arc_file.unwrap());
        let save_file = File::open("testing/savegame.dat");
        let save = saves::read_save::<_, BigEndian>(save_file.unwrap());
    }
}
