/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * - Offroaders123
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

import { gzip } from "pako";
import { CompressionTypes, compressDeflate, compressSwitchPS4RLE, compressVitaRLE, compressZlib } from "../index.js";
import { bWriter } from "binaryio.js";

export function compressSave(save: Uint8Array | ArrayBuffer, compType: CompressionTypes, lEndian = false): Uint8Array {
    /** decompressed size as bigint */
    const fileSize = BigInt(save.byteLength)

    let compressedFile = new Uint8Array([]);

    switch (compType) {
        case CompressionTypes.zlib:
            compressedFile = compressZlib(save);
            break;
        case CompressionTypes.deflate:
            compressedFile = compressDeflate(save);
            break;
        case CompressionTypes.vitarle:
            compressedFile = compressVitaRLE(new Uint8Array(save));
            break;
        case CompressionTypes.switchps4rle:
            // they do not compress saves with this by default.
            compressedFile = compressSwitchPS4RLE(new Uint8Array(save));
        case CompressionTypes.lzx:
            break;
        case CompressionTypes.gzip:
            gzip(new Uint8Array(save));
            break;
        default:
            break;
        case CompressionTypes.none:
            return new Uint8Array(save);
    }

    const compWriter = new bWriter(new Uint8Array(8 + compressedFile.length), lEndian);
    if (compType !== CompressionTypes.vitarle) {
        compWriter.writeLong(fileSize, lEndian);
    } else {
        compWriter.writeInt(0, lEndian);
        compWriter.writeInt(save.byteLength, lEndian);
    }

    for (let i = 0; i < compressedFile.length; i++) {
        compWriter.writeByte(compressedFile[i]!);
    }

    return new Uint8Array(compWriter.arrayBuffer);
}