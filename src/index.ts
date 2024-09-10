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

// compression
// maybe name these inflate, deflate.
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
// consoles
export * from "./console/consoles.js";
// msscmp
export * from "./msscmp/readMSSCMP.js";

export interface msscmpFile {
    fileName: string,
    fileSize: number,
    sampleRate: number
    file: File;
}

export interface worldInfo {
    "4J_SEED": string,
    "4J_HOSTOPTIONS": string,
    "4J_TEXTUREPACK": string,
    "4J_EXTRADATA": string,
    "4J_#LOADS": string
}

export interface World {
    name?: string,
    thumbnail: File,
    worldInfo: worldInfo
}

export interface save {
    indexOffset: number,
    fileCount: number,
    fileMinVerSupported: number,
    fileVersion: number,
    fileIndex: index[]
}

export interface arc {
    fileCount: number,
    fileIndex: ArcIndex[]
}

export interface ArcIndex {
    name: string,
    length: number,
    offset: number,
    data: File
}

export interface index {
    name: string,
    length: number,
    offset: number,
    timestamp: bigint,
    data: File
}

export enum endianness {
    big,
    little
}

// https://github.com/Team-Lodestone/Documentation/blob/main/LCE/LCE%20File%20Versions.md
export enum saveVersion {
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

export enum consoleTypes {
    Xbox360,
    XboxOne,
    PS3,
    Vita,
    PS4,
    WiiU,
    Switch
}

export enum compressionTypes {
    gzip = 1,
    zlib,
    deflate,
    vitarle,
    switchps4rle,
    lzx,
    none
}