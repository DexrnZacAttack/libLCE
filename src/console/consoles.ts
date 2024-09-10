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

import {compressionTypes, consoleTypes, endianness} from "../index.js";

interface consoleCompression {
    saveCompression: compressionTypes,
    regionCompression: compressionTypes,
    chunkCompression: compressionTypes
}

export function getSaveCompression(console: consoleTypes) {
    const compTypesMap = new Map([
        [consoleTypes.Xbox360, compressionTypes.lzx],
        [consoleTypes.XboxOne, compressionTypes.zlib],
        [consoleTypes.PS3, compressionTypes.deflate],
        [consoleTypes.Vita, compressionTypes.vitarle],
        [consoleTypes.PS4, compressionTypes.zlib],
        [consoleTypes.WiiU, compressionTypes.zlib],
        [consoleTypes.Switch, compressionTypes.none]
    ])
    return compTypesMap.get(console) || compressionTypes.none;
}

export function getCompression(console: consoleTypes): consoleCompression {
    const compTypesMap = new Map<consoleTypes, consoleCompression>([
        [consoleTypes.Xbox360, {
            saveCompression: compressionTypes.lzx,
            regionCompression: compressionTypes.none,
            chunkCompression: compressionTypes.lzx
        }],
        [consoleTypes.XboxOne, {
            saveCompression: compressionTypes.zlib,
            regionCompression: compressionTypes.none,
            chunkCompression: compressionTypes.zlib
        }],
        [consoleTypes.PS3, {
            saveCompression: compressionTypes.deflate,
            regionCompression: compressionTypes.none,
            chunkCompression: compressionTypes.deflate
        }],
        [consoleTypes.Vita, {
            saveCompression: compressionTypes.vitarle,
            regionCompression: compressionTypes.none,
            chunkCompression: compressionTypes.zlib
        }],
        [consoleTypes.PS4, {
            saveCompression: compressionTypes.zlib,
            regionCompression: compressionTypes.none,
            chunkCompression: compressionTypes.zlib
        }],
        [consoleTypes.WiiU, {
            saveCompression: compressionTypes.zlib,
            regionCompression: compressionTypes.none,
            chunkCompression: compressionTypes.zlib
        }],
        [consoleTypes.Switch, {
            saveCompression: compressionTypes.zlib,
            regionCompression: compressionTypes.switchps4rle,
            chunkCompression: compressionTypes.zlib
        }]
    ]);

    return compTypesMap.get(console) || {
        saveCompression: compressionTypes.none,
        regionCompression: compressionTypes.none,
        chunkCompression: compressionTypes.none
    };
}

export function getEndianness(console: consoleTypes) {
    const endiannessMap = new Map([
        [consoleTypes.Xbox360, endianness.big],
        [consoleTypes.XboxOne, endianness.little],
        [consoleTypes.PS3, endianness.big],
        [consoleTypes.Vita, endianness.little],
        [consoleTypes.PS4, endianness.little],
        [consoleTypes.WiiU, endianness.big],
        [consoleTypes.Switch, endianness.little]
    ])
    return endiannessMap.get(console) || endianness.big;
}

export function getImageOffset(console: consoleTypes) {
    const imageOffsets = new Map([
        [consoleTypes.Xbox360, 0], // doesn't use
        [consoleTypes.XboxOne, 0], // unk
        [consoleTypes.PS3, 0], // unk
        [consoleTypes.Vita, 0], // unk
        [consoleTypes.PS4, 0],
        [consoleTypes.WiiU, 256],
        [consoleTypes.Switch, 0] // unk
    ])
    return imageOffsets.get(console) || 0;
}