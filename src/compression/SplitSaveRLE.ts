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

import {bReader, bWriter} from "binaryio.js";

/**
 * This code is Zugebot (jerrinth3glitch)'s code ported to TS.
 * https://github.com/zugebot/LegacyEditor
*/

/**
 * Decompresses the RLE used by split saves.
 * @param data The compressed data
 * @returns The decompressed data
*/
// Note: This is ported from LegacyEditor's rle_nsxps4.cpp
export function decompressSplitSaveRLE(data: Uint8Array): Uint8Array {
    const decompressedLength = data.byteLength;
    const writer: number[] = [];
    const reader = new bReader(data);

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

/**
 * Compresses with the RLE used by Split Saves
 * @param data Data to compress
 * @returns Compressed data
*/
export function compressSplitSaveRLE(data: Uint8Array): Uint8Array {
    const writer: number[] = [];
    let i = 0;

    while (i < data.length) {
        let value = data[i];
        if (value !== 0) {
            writer.push(value!);
            i++;
            continue;
        }

        let runCount = 1;
        while (i + runCount < data.length && data[i + runCount] === 0) {
            runCount++;
        }

        if (runCount < 256) {
            writer.push(0);
            writer.push(runCount);
        } else {
            writer.push(0);
            writer.push(0);
            writer.push((runCount >> 8) - 1); 
            writer.push(runCount & 0xFF); 
        }

        i += runCount;
    }

    return new Uint8Array(writer);
}
