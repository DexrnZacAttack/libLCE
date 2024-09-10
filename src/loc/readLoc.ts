/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * 
 * Licensed under the MIT License. See LICENSE file for details.
*/

import { bReader } from "binaryio.js";


export async function readLoc(locFile: File, lEndian = false) {

    let reader = new bReader(await locFile.arrayBuffer());
    // TODO: write this after learning the format.
}