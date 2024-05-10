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

import { decompressZlib, index } from "../index.js";

export async function readSave(saveFile: File, lEndian = false): Promise<index[]> {
    let save: index[] = []

    let sParserDV = new DataView(await saveFile.arrayBuffer());

    let compressed: boolean = false;
    let decompressedSize: number = 0;
    try {
        if (decompressZlib(sParserDV.buffer.slice(8))) {
            
            compressed = true;
            decompressedSize = Number(sParserDV.getBigUint64(0, lEndian));
            if (compressed === true && (decompressedSize ?? 0) !== 0) {
                sParserDV = new DataView(decompressZlib(sParserDV.buffer.slice(8)).buffer);
            }
        }
    } catch {
        console.log("ZLib decompression failed, file probably isnt compressed.");
    }

    /** Keeps track of where we are in the DV stream. */
    let currentStreamOffset = 0;

    /** Where the index is located in the file */
    const indexOffset = sParserDV.getUint32(currentStreamOffset, lEndian);
    currentStreamOffset += 4;

    /** How many files are located in the index */
    const indexCount = sParserDV.getUint32(currentStreamOffset, lEndian);
    currentStreamOffset += 4 + indexOffset - 8;

    for (var i = 0; i < indexCount; i++) {
        while (currentStreamOffset + 144 <= sParserDV.byteLength) {
            const bytes = sParserDV.buffer.slice(currentStreamOffset, currentStreamOffset + 144);

            /** Name of the file in the index */
            const fileName = new TextDecoder(lEndian === true ? 'utf-16le' : 'utf-16be').decode(bytes.slice(0, 128)).replace(/\0+$/, '');
            currentStreamOffset += 128;
            const fileLength = sParserDV.getUint32(currentStreamOffset, lEndian);
            currentStreamOffset += 4;
            const fileOffset = sParserDV.getUint32(currentStreamOffset, lEndian);
            currentStreamOffset += 4;
            const fileTimestamp = Number(sParserDV.getBigUint64(currentStreamOffset, lEndian));
            currentStreamOffset += 8;
            const fileData: ArrayBuffer = sParserDV.buffer.slice(fileOffset, fileOffset + fileLength);
            save.push({"name": fileName, "length": fileLength, "offset": fileOffset, "timestamp": fileTimestamp, "data": new File( [new Blob( [ new Uint8Array(fileData) ] )], fileName )})
        }
    }

    return save;
}