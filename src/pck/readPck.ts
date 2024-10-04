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
 * - NessieHax
 * - PhoenixARC
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

import { bReader } from "binaryio.js";
import { LookupTable, PckFileData, PckFile, PckFileEntry, PckKV, PckFileTypes } from "../index.js";

export async function readPCK(file: File, isLittle = false): Promise<PckFile> {
    const reader = new bReader(await file.arrayBuffer(), isLittle);

    const version = reader.readUInt();

    const lookupTable: LookupTable[] = [];
    const fileTable: PckFileEntry[] = [];
    const PckFiles: PckFileData[] = [];

    const lookupTableCount = reader.readUInt();

    for (var i = 0; i < lookupTableCount; i++) {
        lookupTable.push({ offset: reader.readUInt(), name: reader.readString16(reader.readUInt() * 2) });
        reader.readUInt();
    }

    const fileTableCount = reader.readUInt();

    for (var i = 0; i < fileTableCount; i++) {
        fileTable.push({ size: reader.readUInt(), type: reader.readUInt() as PckFileTypes, name: reader.readString16(reader.readUInt() * 2) });
        reader.readUInt();
    }

    for (var i = 0; i < fileTableCount; i++) {
        const KVTable: PckKV[] = [];
        var propertyCount = reader.readUInt();
        for (var j = 0; j < propertyCount; j++) {
            const key = reader.readUInt();
            const value = reader.readString16(reader.readUInt() * 2);
            reader.readUInt();
            KVTable.push({key, value});
        }
        
        const data = new Uint8Array(reader.read(fileTable[i]?.size!));
        PckFiles.push({ file: fileTable[i]!, fileKV: KVTable, data });
    }

    return {
        version,
        lookupTable: lookupTable,
        fileTable: PckFiles
    }
}