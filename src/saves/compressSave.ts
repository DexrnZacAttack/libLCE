/**
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
import { compressionTypes, compressRLE, compressVitaRLE, compressZlib } from "../index.js";
import { bWriter } from "../io/bWriter.js";

export async function compressSave(file: File, compType: compressionTypes, lEndian = false): Promise<File> {
    // arraybuffer variable
    const fileArray = await file.arrayBuffer();
    const fileArrayBuffer = new Uint8Array(fileArray);
    
    /** decompressed size as bigint */
    const fileSize = BigInt(fileArray.byteLength)

    let compressedFile = new Uint8Array(fileArray);

    switch (compType) {
        case compressionTypes.zlib:
            compressedFile = compressZlib(fileArray);
            break;
        case compressionTypes.rle:
            compressedFile = compressRLE(fileArray);
            break;
        case compressionTypes.vitarle:
            compressedFile = compressVitaRLE(new Uint8Array(fileArray));
            break;
        case compressionTypes.switchrle:
            break;
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

    /** allocate a dataview for the compressed data */
    const compWriter = new bWriter(new DataView(new Uint8Array([...fileArrayBuffer, ...compressedFile]).buffer), lEndian);
    compWriter.writeLong(fileSize, lEndian)

    compressedFile.forEach(i => {
        compWriter.writeByte(compressedFile[i]!);
    });

    return new File([new Blob([compWriter.getBuffer()])], file.name);
}