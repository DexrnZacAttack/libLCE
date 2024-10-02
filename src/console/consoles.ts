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

import {CompressionTypes, ConsoleTypes, Endianness} from "../index.js";

interface consoleCompression {
    saveCompression: CompressionTypes,
    regionCompression: CompressionTypes,
    chunkCompression: CompressionTypes
}

export function getSaveCompression(console: ConsoleTypes) {
    const compTypesMap = new Map([
        [ConsoleTypes.Xbox360, CompressionTypes.lzx],
        [ConsoleTypes.XboxOne, CompressionTypes.zlib],
        [ConsoleTypes.PS3, CompressionTypes.deflate],
        [ConsoleTypes.Vita, CompressionTypes.vitarle],
        [ConsoleTypes.PS4, CompressionTypes.zlib],
        [ConsoleTypes.WiiU, CompressionTypes.zlib],
        [ConsoleTypes.Switch, CompressionTypes.none]
    ])
    return compTypesMap.get(console) || CompressionTypes.none;
}

export function getCompression(console: ConsoleTypes): consoleCompression {
    const compTypesMap = new Map<ConsoleTypes, consoleCompression>([
        [ConsoleTypes.Xbox360, {
            saveCompression: CompressionTypes.lzx,
            regionCompression: CompressionTypes.none,
            chunkCompression: CompressionTypes.lzx
        }],
        [ConsoleTypes.XboxOne, {
            saveCompression: CompressionTypes.zlib,
            regionCompression: CompressionTypes.none,
            chunkCompression: CompressionTypes.zlib
        }],
        [ConsoleTypes.PS3, {
            saveCompression: CompressionTypes.deflate,
            regionCompression: CompressionTypes.none,
            chunkCompression: CompressionTypes.deflate
        }],
        [ConsoleTypes.Vita, {
            saveCompression: CompressionTypes.vitarle,
            regionCompression: CompressionTypes.none,
            chunkCompression: CompressionTypes.zlib
        }],
        [ConsoleTypes.PS4, {
            saveCompression: CompressionTypes.zlib,
            regionCompression: CompressionTypes.none,
            chunkCompression: CompressionTypes.zlib
        }],
        [ConsoleTypes.WiiU, {
            saveCompression: CompressionTypes.zlib,
            regionCompression: CompressionTypes.none,
            chunkCompression: CompressionTypes.zlib
        }],
        [ConsoleTypes.Switch, {
            saveCompression: CompressionTypes.zlib,
            regionCompression: CompressionTypes.switchps4rle,
            chunkCompression: CompressionTypes.zlib
        }]
    ]);

    return compTypesMap.get(console) || {
        saveCompression: CompressionTypes.none,
        regionCompression: CompressionTypes.none,
        chunkCompression: CompressionTypes.none
    };
}

export function getEndianness(console: ConsoleTypes) {
    const endiannessMap = new Map([
        [ConsoleTypes.Xbox360, Endianness.big],
        [ConsoleTypes.XboxOne, Endianness.little],
        [ConsoleTypes.PS3, Endianness.big],
        [ConsoleTypes.Vita, Endianness.little],
        [ConsoleTypes.PS4, Endianness.little],
        [ConsoleTypes.WiiU, Endianness.big],
        [ConsoleTypes.Switch, Endianness.little]
    ])
    return endiannessMap.get(console) || Endianness.big;
}

export function getImageOffset(console: ConsoleTypes) {
    const imageOffsets = new Map([
        [ConsoleTypes.Xbox360, 0], // doesn't use
        [ConsoleTypes.XboxOne, 0], // unk
        [ConsoleTypes.PS3, 0], // unk
        [ConsoleTypes.Vita, 0], // unk
        [ConsoleTypes.PS4, 0],
        [ConsoleTypes.WiiU, 256],
        [ConsoleTypes.Switch, 0] // unk
    ])
    return imageOffsets.get(console) || 0;
}