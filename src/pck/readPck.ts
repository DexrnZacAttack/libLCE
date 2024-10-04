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

interface LookupTable {
    offset: number;
    name: string;
};

enum PckFileTypes {
    Skin,
    Cape,
    Texture,
    UiInfo,
    Info,
    TexturePackInfo,
    Localization,
    GameRules,
    Audio,
    Colors,
    GameRulesHeader,
    SkinData,
    Models,
    Behaviors,
    Material
};

interface FileEntry {
    size: number;
    type: PckFileTypes;
    name: string;
};

interface FileDataEntry {
    key: number;
    name: string;
    data: Uint8Array;
};

export async function readPCK(file: File, isLittle = false) {
    const reader = new bReader(await file.arrayBuffer(), isLittle);

    const pckType = reader.readUInt();

    const lookupTable: LookupTable[] = [];
    const fileTable: FileEntry[] = [];
    const fileDataTable: FileDataEntry[] = [];

    const lookupTableCount = reader.readUInt();

    console.log(reader.pos);
    for (var i = 0; i < lookupTableCount; i++) {
        lookupTable.push({offset: reader.readUInt(), name: reader.readString16(reader.readUInt() * 2)});
        reader.readUInt();
    }

    const fileTableCount = reader.readUInt();

    for (var i = 0; i < fileTableCount; i++) {
        fileTable.push({size: reader.readUInt(), type: reader.readUInt() as PckFileTypes, name: reader.readString16(reader.readUInt() * 2)});
        reader.readUInt();
    }

    const fileDataTableCount = reader.readUInt();

    for (var i = 0; i < lookupTableCount; i++) {
        const key = reader.readUInt();
        const name = reader.readString16(reader.readUInt() * 2);
        console.log(fileTable[lookupTable[i]?.offset!]?.size)
        console.log(lookupTable[i]?.name!);
    }
}