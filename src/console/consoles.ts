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