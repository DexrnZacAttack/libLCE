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

import { saveVersion } from "../index.js";
import { bWriter } from "../io/bWriter.js";

/**
 * This is the number of bytes that the header takes up, which is 12 (4 bytes offset, 4 bytes count, 2 bytes minimum version, 2 bytes file version)
*/
const headerLength: number = 12;

/**
 * This is the number of bytes that an entry in the index takes up, which is 144 (136 for pre-release)
*/
let indexEntryLength: number = 144

/**
 * This contains the current file's name.
 * When setting this, I switch the paths from Windows style to Unix style as that's how the save format does folders.
 */
let uFileName!: string;

interface saveOptions {
    verMinimum: number;
    verCurrent: number;
}

/** Generates a savefile.
 * @param files Array of files to put in the savegame.
 * @param lEndian Whether to use Little endian or not, default is no.
 */
export function generateSave(files: [File, Buffer][], lEndian: boolean = false, saveOptions: saveOptions = {"verMinimum": 11, "verCurrent": 11}): File {
    /** Determines whether or not the save file is ver 0-1 which has a slightly different format. */
    const isPreReleaseSF = saveOptions.verCurrent == saveVersion.TU0033;

    /*
     * This is used to keep track of what file we are on... only used in one place though. (sgCurrentFileOffset)
    */
    let fIndex: number = 0;
    
    /**
     * This is the number of bytes (the length) of every file combined.
    */
    const filesLength: number = files.reduce(
        (previous, [, file]) => previous + file.byteLength,
        0
    );

    /**
     * This is the first part of the 8 byte header containing the offset of the index in the savegame... the index is what holds all of the file names and their info.
    */
    const offset: number = filesLength + headerLength;

    /**
     * This is the second part of the 8 byte header containing the number of files that is in the index.
    */
    let count: number = files.length;

    if (isPreReleaseSF) {
        indexEntryLength = 136;
    }

    console.log(`There are ${count} files in the folder, of which take up ${filesLength} bytes space.`);
    
    /**
     * This is the DataView object that contains the bytes of the savegame that we are creating.
    */
    const saveWriter = new bWriter(new DataView(new ArrayBuffer(filesLength + headerLength + indexEntryLength * count)), lEndian);

    // Write offset and count to start of file
    saveWriter.writeUInt(offset, lEndian);
    saveWriter.writeUInt((saveOptions.verCurrent < 2 ? count * indexEntryLength : count), lEndian);
    // https://github.com/zugebot/legacyeditor for both of these 2 shorts.
    saveWriter.writeUShort(saveOptions.verMinimum, lEndian);
    saveWriter.writeUShort(saveOptions.verCurrent, lEndian);

    /**
     * For each file in the index, we keep an offset that says where the file starts, we use fIndex to see each file's offset.
    */
        let sgCurrentFileOffset: Array<number> = [];
        // Write the files to the save.
        for (const [fileObj, file] of files) {
            uFileName = fileObj.name.replace("\\", "/");
            if (uFileName.length !== 0) {
                sgCurrentFileOffset.push(saveWriter.pos);
                console.log(`Writing ${uFileName}...`);
                // for every byte in the file, write said byte.
                file.forEach((byte) => {
                    saveWriter.writeByte(byte);
                });
            } else {
                // if the file's name is blank... don't bother writing it, as there may be something wrong with the file.
                console.log("File has no name... skipping!");
            }
        }

    // Write index entries
    for (const [fileObj, file] of files) {

        uFileName = fileObj.name.replace("\\", "/");
        // if the filename doesn't have a name, don't write it... otherwise write it.
        if (uFileName.length !== 0) {
            console.log(`Writing "${uFileName}" to index...`);
            // Write the file name in UTF16
            saveWriter.writeString16(uFileName);
            // add a shit ton of 0s before adding the rest of the info
            for (var i: number = 0; i < 128 - uFileName.length * 2; i++) {
                saveWriter.writeByte(0);
            }
            // File Length
            saveWriter.writeUInt(file.length ?? 0);

            // File Offset
            if (file.length !== 0)
                saveWriter.writeUInt(sgCurrentFileOffset[fIndex] ?? 0);
            else
                saveWriter.writeUInt(0);
            
            if (!isPreReleaseSF) {
                // Timestamp, not in the same format (consoles write some weird one based on reset time, or other factors)
                saveWriter.writeULong(BigInt(Date.now()));
            }
            fIndex++;
        } else {
            console.warn("File has no name... unable to add to index!");
        }
    }
    return new File([new Blob([saveWriter.buffer])], 'savegame.dat');
}
