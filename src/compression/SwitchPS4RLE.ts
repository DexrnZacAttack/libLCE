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

import { bReader } from "../io/bReader.js";
import { bWriter } from "../io/bWriter.js";

/**
 * This code is Zugebot (jerrinth3glitch)'s code ported to TS.
 * https://github.com/zugebot/LegacyEditor
*/

/**
 * Decompresses Switch/PS4 RLE
 * @param data The compressed data
 * @returns The decompressed data
*/
// Note: This is ported from LegacyEditor's rle_nsxps4.cpp
export function decompressSwitchPS4RLE(data: Uint8Array): Uint8Array | undefined {
    const decompressedLength = data.byteLength;
    const writer: number[] = [];
    const reader = new bReader(new DataView(data.buffer));

    while (reader.getPos() < decompressedLength) {
        let byte = reader.readByte();
        if (byte != 0) {
            writer.push(byte);
        } else {
            let zeroCount = reader.readByte();

            if (zeroCount == 0) {
                const zeroCount1 = reader.readByte();
                const zeroCount2 = reader.readByte();
                zeroCount = zeroCount1 << 8 | zeroCount2;
                zeroCount += 256;
            }

            for (let i = 0; i < zeroCount; i++) {
                writer.push(0);
            }
        }
    }
    return new Uint8Array(writer);
} 

// TODO: remove these getSize functions, they are janky.
export function getCompressedSize(data: Uint8Array): number {
    let compressedSize = 0;

    const reader = new bReader(new DataView(data.buffer));

    while (reader.getPos() < data.length) {
        let value;
        if ((value = reader.readByte()) != 0) {
            compressedSize++;
            continue;
        }

        let runCount = 1;
        while (reader.getPos() + runCount < data.length && data[reader.getPos() + runCount] == 0) {
            runCount++;
        }

        if (runCount < 256) {
            compressedSize += 2;
        } else {
            compressedSize += 4;
        }

        reader.curPos += runCount;
    }

    return compressedSize;
}


/**
 * Compresses with Switch/PS4 RLE
 * @param data Data to compress
 * @returns Compressed data
*/
// Note: This is ported from LegacyEditor's rle_nsxps4.cpp
export function compressSwitchPS4RLE(data: Uint8Array): Uint8Array {
    const writer = new bWriter(new DataView(new ArrayBuffer(getCompressedSize(data))));
    const reader = new bReader(new DataView(data.buffer));

    while (reader.getPos() < data.length) {
        let value;
        if ((value = reader.readByte()) != 0) {
            writer.writeByte(value!);
            continue;
        }

        let runCount = 1;
        while (reader.getPos() + runCount < data.length && data[reader.getPos() + runCount] == 0) {
            runCount++;
        }

        if (runCount < 256) {
            writer.writeByte(0);
            writer.writeByte(runCount);
        } else {
            writer.writeByte(0);
            writer.writeByte(0);
            writer.writeByte((runCount >> 8) - 1);
            writer.writeByte(runCount & 255);
        }

        reader.curPos += runCount;
    }
    return new Uint8Array(writer.getBuffer());
}