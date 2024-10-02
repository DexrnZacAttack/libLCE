/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * - Offroaders123
 * 
 * Other credits:
 * https://github.com/zugebot/LegacyEditor
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

import { decompressZlib, SaveIndex, Savegame, SaveVersion } from "../index.js";
import { bReader } from 'binaryio.js';

interface readOptions {
    /** Ignores the version parameter which is used for endianness detection. */
    ignoreVersion?: boolean
}

export async function readSave(saveFile: File, lEndian = false, ro: readOptions = {ignoreVersion: false}): Promise<Savegame> {
    const index: SaveIndex[] = []

    let saveReader = new bReader(await saveFile.arrayBuffer(), lEndian);

    let decompressedSize: number = 0;
    try {
        const decompressTest = new Uint8Array(saveReader.slice(8));
        if (decompressTest[0] === 0x78 && decompressTest[1] === 0x9C) {
            if (decompressZlib(decompressTest)) {
                decompressedSize = Number(saveReader.readULong());
                if ((decompressedSize ?? 0) !== 0) {
                    saveReader = new bReader(decompressZlib(decompressTest), lEndian);
                }
            }
        } else {
            console.log(`No ZLib header, not gonna attempt decompress.`)
        }
    } catch (e) {
        console.warn("ZLib decompression failed, library returned message: " + e);
        console.log("This may be intended behavior if the file is indeed not compressed, but unlikely as the ZLib header check passed.");
    }

    if (!ro.ignoreVersion) {
        saveReader.setPos(10);
        const ver = saveReader.readUShort();
        // it works though...
        if (ver > 20) {
            console.warn(`Invalid version ${ver}, trying with reverse endian... To disable this behavior, you need to have ignoreVersion set to true in the ro param.`);
            saveReader.setEndianness(!lEndian);
        }
        saveReader.setPos(0);
    }

    /** Where the index is located in the file */
    const indexOffset = saveReader.readUInt();

    
    /** How many files are located in the index */
    let indexCount = saveReader.readUInt();

    // https://github.com/zugebot/legacyeditor for these 2 shorts
    /** Minimum supported LCE version */
    const fileMinimumVersion = saveReader.readUShort();
    /** Current LCE file version */
    const fileVersion = saveReader.readUShort();

    let indexEntrySize = 144;

    /** Determines whether the save file is ver 0-1 which has a slightly different format. */
    const isPreReleaseSF = fileVersion == SaveVersion.TU0033;

    if (isPreReleaseSF) {
        // first 2 pr versions have count in bytes.
        indexEntrySize = 136;
        indexCount = indexCount / indexEntrySize;
    }
    
    saveReader.incrementPos(indexOffset - 12);

    for (var i = 0; i < indexCount; i++) {
        while (saveReader.pos + indexEntrySize <= saveReader.byteLength) {
            /** Name of the file in the index */
            const fileName = saveReader.readString16(128);
            /** Length of file in the index */
            const fileLength = saveReader.readUInt();
            /** Location (Offset) of the file in the index */
            const fileOffset = saveReader.readUInt();

            /** Timestamp of the file (unusable due to how it's written.) */
            let fileTimestamp = 0n;
            if (!isPreReleaseSF) {
                // first 2 pr versions don't include timestamp
                fileTimestamp = saveReader.readULong() as bigint;
            }
            
            const fileData: ArrayBuffer = saveReader.slice(fileOffset, fileOffset + fileLength);
            index.push({"name": fileName, "length": fileLength, "offset": fileOffset, "timestamp": fileTimestamp, "data": new File( [new Blob( [ fileData ] )], fileName )})
        }
    }

    return {"indexOffset": indexOffset, "fileCount": indexCount, "fileMinVerSupported": fileMinimumVersion, "fileVersion": fileVersion, "fileIndex": index};
}