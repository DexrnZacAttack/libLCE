/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * 
 * Other credits:
 * https://github.com/zugebot/LegacyEditor
 *
 * Licensed under the MIT License. See LICENSE file for details.
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

    while (reader.pos < decompressedLength) {
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

    while (reader.pos < data.length) {
        if (reader.readByte() != 0) {
            compressedSize++;
            continue;
        }

        let runCount = 1;
        while (reader.pos + runCount < data.length && data[reader.pos + runCount] == 0) {
            runCount++;
        }

        if (runCount < 256) {
            compressedSize += 2;
        } else {
            compressedSize += 4;
        }

        reader.incrementPos(runCount);
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

    while (reader.pos < data.length) {
        let value;
        if ((value = reader.readByte()) != 0) {
            writer.writeByte(value!);
            continue;
        }

        let runCount = 1;
        while (reader.pos + runCount < data.length && data[reader.pos + runCount] == 0) {
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

        reader.incrementPos(runCount);
    }
    return new Uint8Array(writer.buffer);
}