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

import { consoleTypes, compressionTypes, endianness } from "../index.js";
 
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