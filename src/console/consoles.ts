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
        [ConsoleTypes.Xbox360, CompressionTypes.Lzx],
        [ConsoleTypes.XboxOne, CompressionTypes.Zlib],
        [ConsoleTypes.PS3, CompressionTypes.Deflate],
        [ConsoleTypes.Vita, CompressionTypes.VitaRle],
        [ConsoleTypes.PS4, CompressionTypes.Zlib],
        [ConsoleTypes.WiiU, CompressionTypes.Zlib],
        [ConsoleTypes.Switch, CompressionTypes.None]
    ])
    return compTypesMap.get(console) || CompressionTypes.None;
}

export function getCompression(console: ConsoleTypes): consoleCompression {
    const compTypesMap = new Map<ConsoleTypes, consoleCompression>([
        [ConsoleTypes.Xbox360, {
            saveCompression: CompressionTypes.Lzx,
            regionCompression: CompressionTypes.None,
            chunkCompression: CompressionTypes.Lzx
        }],
        [ConsoleTypes.XboxOne, {
            saveCompression: CompressionTypes.Zlib,
            regionCompression: CompressionTypes.SplitSaveRle,
            chunkCompression: CompressionTypes.Zlib
        }],
        [ConsoleTypes.PS3, {
            saveCompression: CompressionTypes.Deflate,
            regionCompression: CompressionTypes.None,
            chunkCompression: CompressionTypes.Deflate
        }],
        [ConsoleTypes.Vita, {
            saveCompression: CompressionTypes.VitaRle,
            regionCompression: CompressionTypes.None,
            chunkCompression: CompressionTypes.Zlib
        }],
        [ConsoleTypes.PS4, {
            saveCompression: CompressionTypes.Zlib,
            regionCompression: CompressionTypes.SplitSaveRle,
            chunkCompression: CompressionTypes.Zlib
        }],
        [ConsoleTypes.WiiU, {
            saveCompression: CompressionTypes.Zlib,
            regionCompression: CompressionTypes.None,
            chunkCompression: CompressionTypes.Zlib
        }],
        [ConsoleTypes.Switch, {
            saveCompression: CompressionTypes.Zlib,
            regionCompression: CompressionTypes.SplitSaveRle,
            chunkCompression: CompressionTypes.Zlib
        }]
    ]);

    return compTypesMap.get(console) || {
        saveCompression: CompressionTypes.None,
        regionCompression: CompressionTypes.None,
        chunkCompression: CompressionTypes.None
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