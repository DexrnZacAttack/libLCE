/**
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
export { deflate as compressZlib, inflate as decompressZlib, deflateRaw as compressRLE, inflateRaw as decompressRLE } from "pako";
export * from "./compression/VitaRLE.js";
export * from "./compression/SwitchPS4RLE.js";
// saves
export * from "./saves/createSave.js";
export * from "./saves/readSave.js";
export * from "./saves/compressSave.js";
// world
export * from "./world/parseWorldInfo.js";
// loc
export * from "./loc/readLoc.js";

export interface World {
    name: string,
    thumbnail: File
}

export interface save {
    indexOffset: number,
    fileCount: number,
    minVerSupported: number,
    maxVerSupported: number,
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

export enum clientTypes {
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
    rle,
    vitarle,
    switchrle,
    lzx,
    none
}