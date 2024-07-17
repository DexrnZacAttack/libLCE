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

import { gzip } from "pako";
import { compressionTypes, compressDeflate, compressSwitchPS4RLE, compressVitaRLE, compressZlib } from "../index.js";
import { bWriter } from "../io/bWriter.js";

export async function compressSave(file: File, compType: compressionTypes, lEndian = false): Promise<File> {
    const fileArray = await file.arrayBuffer();
    
    /** decompressed size as bigint */
    const fileSize = BigInt(fileArray.byteLength)

    let compressedFile = new Uint8Array([]);

    switch (compType) {
        case compressionTypes.zlib:
            compressedFile = compressZlib(fileArray);
            break;
        case compressionTypes.deflate:
            compressedFile = compressDeflate(fileArray);
            break;
        case compressionTypes.vitarle:
            compressedFile = compressVitaRLE(new Uint8Array(fileArray));
            break;
        case compressionTypes.switchps4rle:
            // they do not compress saves with this by default.
            compressedFile = compressSwitchPS4RLE(new Uint8Array(fileArray));
        case compressionTypes.lzx:
            break;
        case compressionTypes.gzip:
            gzip(new Uint8Array(fileArray));
            break;
        default:
            break;
        case compressionTypes.none:
            return new File([new Blob([fileArray])], file.name);
    }

    // I was stumped for about ~2 days wondering why my compression stuff is so messed up... this line here was the culprit.
    const compWriter = new bWriter(new DataView(new Uint8Array(8 + compressedFile.length).buffer), lEndian);
    if (compType !== compressionTypes.vitarle) {
        compWriter.writeLong(fileSize, lEndian);
    } else {
        compWriter.writeInt(0, lEndian);
        compWriter.writeInt(fileArray.byteLength, lEndian);
    }

    for (let i = 0; i < compressedFile.length; i++) {
        compWriter.writeByte(compressedFile[i]!);
    }

    return new File([new Blob([compWriter.buffer])], file.name);
}