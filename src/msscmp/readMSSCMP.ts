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

import {msscmpFile} from "../index.js";
import {bReader} from "binaryio.js";

export async function readMSSCMP(file: File, newGen?: boolean, lEndian?: boolean): Promise<msscmpFile[]> {

    const files: msscmpFile[] = [];
    const reader = new bReader(await file.arrayBuffer(), lEndian);

    const magic = reader.readString8(4);
    if (magic == "BANK" || magic == "KNAB") {
        if (typeof lEndian === 'undefined') {
            // if lEndian wasn't defined during fn call then it will use magic for endianness.
            lEndian = (magic == "KNAB");
            reader.setEndianness(lEndian);
        }
    } else {
        throw new Error(`Invalid file magic! (expected magic "BANK" or "KNAB", got ${magic} instead)`);
    }

    // shitty way to detect if new gen or not.
    reader.setPos(0x18);
    if (reader.readUInt() === reader.readUInt() && typeof newGen === 'undefined') {
        newGen = false;
    } else {
        newGen = true;
    }

    reader.setPos(0x04);

    reader.readUInt();
    /** Marks the point where the file data starts */
    const fileDataStartOffset = newGen == false ? reader.readUInt() : Number(reader.readULong());

    reader.readULong();

    /** Offset of the first index */
    const firstIndexOffset = newGen == false ? reader.readUInt() : Number(reader.readULong());

    /** Marks the last offset in the first index */
    const lastEntryOffset = newGen == false ? reader.readUInt() : Number(reader.readULong());

    reader.read(newGen == false ? 8 : 16)

    /** seems to always have 0x03000000 at the start (le) */
    const unkOffset = newGen == false ? reader.readUInt() : Number(reader.readULong());

    /** How many entries are in index 1 */
    const index1Count = newGen == false ? reader.readUInt() : Number(reader.readULong());
    
    newGen == false ? reader.readULong() : reader.readUInt();

    /** How many entries are in index 2 */
    const count = reader.readUInt();
    
    /** MSSCMP root name */
    const name = reader.readString8(12);

    console.log(`Reading ${name}`)

    reader.setPos(lastEntryOffset + 4);

    for (var i = 0; i < count; i++) {
        reader.setPos(lastEntryOffset + 4 + (i * 8));
        
        const entryOffset = reader.readUInt();
        let fStructureOffset = 0;
        if (i != count)
            fStructureOffset = reader.readUInt();

        reader.setPos(entryOffset);

        // file info
        const nameOffset = reader.readUInt();
        const curOffset = reader.pos;
        // go to the offset of the name and actually grab it
        reader.setPos(nameOffset);
        const fileName = reader.readNullTerminatedString8();

        // come back
        reader.setPos(curOffset);
        // continue
        reader.readUInt();

        const fileDataOffset = reader.readUInt();
        reader.readUInt(); // dunno
        reader.readUInt(); // dunno
        const sampleRate = reader.readUInt();
        const fileSize = reader.readUInt();

        // don't really care about anything else.
        files.push({fileName: fileName, fileSize: fileSize, sampleRate: sampleRate, file: new File([reader.slice(fileDataOffset, fileDataOffset + fileSize)], fileName)})
    }

    return files;

} 