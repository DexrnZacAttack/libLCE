/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/
mod saves;
mod util;

// I don't know basically anything about rust so it's gonna be a journey

#[cfg(test)]
mod tests {
    use std::fs::File;
    use byteorder::BigEndian;
    use super::*;

    #[test]
    // apache2 debian default page
    fn it_works() {
        let file = File::open("savegame.dat");
        let mut save = saves::read_save::<_, BigEndian>(file.unwrap());
    }
}
