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

export async function compressSave(file: File, compType: CompressionTypes, lEndian = false): Promise<File> {
    const fileArray = await file.arrayBuffer();
    
    /** decompressed size as bigint */
    const fileSize = BigInt(fileArray.byteLength)

    let compressedFile = new Uint8Array([]);

    switch (compType) {
        case CompressionTypes.zlib:
            compressedFile = compressZlib(fileArray);
            break;
        case CompressionTypes.deflate:
            compressedFile = compressDeflate(fileArray);
            break;
        case CompressionTypes.vitarle:
            compressedFile = compressVitaRLE(new Uint8Array(fileArray));
            break;
        case CompressionTypes.switchps4rle:
            // they do not compress saves with this by default.
            compressedFile = compressSwitchPS4RLE(new Uint8Array(fileArray));
        case CompressionTypes.lzx:
            break;
        case CompressionTypes.gzip:
            gzip(new Uint8Array(fileArray));
            break;
        default:
            break;
        case CompressionTypes.none:
            return new File([new Blob([fileArray])], file.name);
    }

    // I was stumped for about ~2 days wondering why my compression stuff is so messed up... this line here was the culprit.
    const compWriter = new bWriter(new Uint8Array(8 + compressedFile.length), lEndian);
    if (compType !== CompressionTypes.vitarle) {
        compWriter.writeLong(fileSize, lEndian);
    } else {
        compWriter.writeInt(0, lEndian);
        compWriter.writeInt(fileArray.byteLength, lEndian);
    }

    for (let i = 0; i < compressedFile.length; i++) {
        compWriter.writeByte(compressedFile[i]!);
    }

    return new File([new Blob([compWriter.arrayBuffer])], file.name);
}