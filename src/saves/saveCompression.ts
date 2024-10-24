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
import { CompressionTypes, compressDeflate, compressSplitSaveRLE, compressVitaRLE, compressZlib, decompressDeflate, decompressSplitSaveRLE, decompressVitaRLE, decompressZlib } from "../index.js";
import { bReader, bWriter } from "binaryio.js";

export function compressSave(data: Uint8Array | ArrayBuffer, compType: CompressionTypes, lEndian = false): Uint8Array {
    /** decompressed size */
    const fileSize = data.byteLength

    let compressedFile = new Uint8Array([]);

    switch (compType) {
        case CompressionTypes.Zlib:
            compressedFile = compressZlib(data);
            break;
        case CompressionTypes.Deflate:
            compressedFile = compressDeflate(data);
            break;
        case CompressionTypes.VitaRle:
            compressedFile = compressVitaRLE(new Uint8Array(data));
            break;
        case CompressionTypes.SplitSaveRle:
            compressedFile = compressSplitSaveRLE(new Uint8Array(data));
            break;
        case CompressionTypes.Lzx:
            break;
        case CompressionTypes.Gzip:
            gzip(new Uint8Array(data));
            break;
        default:
            break;
        case CompressionTypes.None:
            return new Uint8Array(data);
    }

    console.log(compressedFile);

    const compWriter = new bWriter(new Uint8Array((compType == CompressionTypes.SplitSaveRle ? 4 : 8) + compressedFile.length), lEndian);

    switch (compType) {
        case CompressionTypes.VitaRle:
            compWriter.writeLong(BigInt(fileSize), lEndian);
            break;
        case CompressionTypes.SplitSaveRle:
            compWriter.writeInt(fileSize, lEndian);
            break;
        default:
            compWriter.writeInt(0, lEndian);
            compWriter.writeInt(fileSize, lEndian);
            break;
    }
        
    for (let i = 0; i < compressedFile.length; i++) {
        compWriter.writeByte(compressedFile[i]!);
    }

    return new Uint8Array(compWriter.arrayBuffer);
}

export function decompressSave(data: Uint8Array | ArrayBuffer, compType: CompressionTypes, lEndian = false): Uint8Array {
    const reader = new bReader(data, lEndian);
    let decompressed = new Uint8Array([]);

    switch (compType) {
        case CompressionTypes.Zlib:
            reader.readUInt();
            reader.readUInt(); // size
            decompressed = decompressZlib(new Uint8Array(reader.slice(8)));
            break;
        case CompressionTypes.Deflate:
            reader.readUInt();
            reader.readUInt(); // size
            decompressed = decompressDeflate(new Uint8Array(reader.slice(8)));
            break;
        case CompressionTypes.VitaRle:
            reader.readULong(); // size
            decompressed = decompressVitaRLE(new Uint8Array(reader.slice(8)));
            break;
        case CompressionTypes.SplitSaveRle:
            reader.readUInt(); // size
            decompressed = decompressSplitSaveRLE(new Uint8Array(reader.slice(4)));
        case CompressionTypes.Lzx:
            break;
        case CompressionTypes.Gzip:
            break;
        default:
            break;
        case CompressionTypes.None:
            return new Uint8Array(data);
    }

    return decompressed;
}