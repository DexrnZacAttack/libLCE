/*
 * MIT License
 * Copyright (c) 2024 Dexrn ZacAttack
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
export enum fileVersion {
    // version names are equivalent to the first version where the file version is used
    // e.g TU5 is the first version to use file version 3.
    TU0033,
    TU0035,
    TU0054,
    TU5
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