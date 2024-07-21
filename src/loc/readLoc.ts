/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * 
 * Other credits:
 * https://github.com/PhoenixARC/-OMI-Filetype-Library
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/


// unfinished.

export async function readLoc(locFile: File, lEndian = false) {
    // ah it seems omifiletypes reads signed instead of unsigned

    let locParserDV = new DataView(await locFile.arrayBuffer());
    // istg I might just create my own modified dataview thingy that stores a pos internally.
    let curPosition = 0;
    // const version = locParserDV.getInt32(curPosition, lEndian);
    curPosition += 4;
    // const langCount = locParserDV.getInt32(curPosition, lEndian);
    curPosition += 4;
    const useUniqueIDs = locParserDV.getInt8(curPosition);
    curPosition += 1;
    const keyCount = locParserDV.getInt32(curPosition, lEndian);
    curPosition += 4;
    for (var i = 0; i < keyCount - 1; i++) {
        if (useUniqueIDs) {
            // and this is where it ends for now cuz im tired lollll
        }
    }
}