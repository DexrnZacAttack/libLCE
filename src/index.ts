/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * - Offroaders123
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

import { RGBA, RGBColor } from "binaryio.js";

// compression
export { deflate as compressZlib, inflate as decompressZlib, deflateRaw as compressDeflate, inflateRaw as decompressDeflate } from "pako";
export * from "./compression/VitaRLE.js";
export * from "./compression/SwitchPS4RLE.js";
// saves
export * from "./saves/createSave.js";
export * from "./saves/readSave.js";
export * from "./saves/compressSave.js";
// world
export * from "./world/readWorldInfo.js";
// loc
export * from "./loc/readLoc.js";
// arc
export * from "./arc/readArc.js";
export * from "./arc/writeArc.js";
// msscmp
export * from "./msscmp/readMsscmp.js";
// pck
export * from "./pck/readPck.js";
// col
export * from "./col/readCol.js";
export * from "./col/writeCol.js";
// consoles
export * from "./console/consoles.js";

export interface MsscmpFile {
    fileName: string,
    fileSize: number,
    sampleRate: number
    file: File;
}

export interface LookupTable {
    offset: number;
    name: string;
};

export interface LangId {
    name: string;
    unk: number;
}


export interface Keys {
    useUniqueIDs: boolean;
    count: number;
    keys: number[];
}

export interface Loc {
    version: number;
    count: number;
    keys?: Keys;
    languageIds: LangId[];
    languages: Language[];
};

export interface Language {
    shouldReadByte: number;
    byte?: number;
    language: string;
    stringCount: number;
    strings: string[];
};

export enum PckFileTypes {
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

export interface PckFileEntry {
    size: number;
    type: PckFileTypes;
    name: string;
};

export interface PckKV {
    key: number;
    value: string;
};

export interface PckFileData {
    file: PckFileEntry;
    fileKV: PckKV[];
    data: Uint8Array;
};

export interface PckFile {
    version: number;
    xmlVersion?: number;
    lookupTable: LookupTable[]
    fileTable: PckFileData[];
};

export interface WorldInfo {
    "4J_SEED": string,
    "4J_HOSTOPTIONS": string,
    "4J_TEXTUREPACK": string,
    "4J_EXTRADATA": string,
    "4J_#LOADS": string
    "4J_BASESAVENAME": string
}

export interface World {
    name?: string,
    thumbnail: Uint8Array,
    worldInfo: WorldInfo
}

export interface Savegame {
    indexOffset: number,
    fileCount: number,
    fileMinVerSupported: number,
    fileVersion: number,
    fileIndex: SaveIndex[]
}

export interface Arc {
    fileCount: number,
    fileIndex: ArcIndex[]
}

export interface ColColor {
    name: string;
    color: RGBColor;
};

export interface ColWorldColor {
    name: string;
    waterColor: RGBColor;
    underwaterColor: RGBColor;
    fogColor: RGBColor;
};

export interface Col {
    version: number;
    colors: ColColor[];
    worldColors?: ColWorldColor[];
}

export interface ArcIndex {
    name: string,
    length: number,
    offset: number,
    data: File
}

export interface SaveIndex {
    name: string,
    length: number,
    offset: number,
    timestamp: bigint,
    data: File
}

export enum Endianness {
    big,
    little
}

// https://github.com/Team-Lodestone/Documentation/blob/main/LCE/LCE%20File%20Versions.md
export enum SaveVersion {
    // version names are equivalent to the first version where the file version is used
    // e.g TU5 is the first version to use file version 3.
    TU0033 = 1,
    TU0054 = 2,
    TU5 = 3,
    TU9 = 5,
    TU14 = 6,
    TU17 = 8,
    TU19 = 9,
    TU36 = 10,
    TU69 = 11
}

export enum ConsoleTypes {
    Xbox360,
    XboxOne,
    PS3,
    Vita,
    PS4,
    WiiU,
    Switch
}

export enum CompressionTypes {
    gzip = 1,
    zlib,
    deflate,
    vitarle,
    switchps4rle,
    lzx,
    none
}