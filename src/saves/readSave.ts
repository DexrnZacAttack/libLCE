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
import { bReader } from "../io/bReader.js";

export async function readSave(saveFile: File, lEndian = false): Promise<index[]> {
    let save: index[] = []

    let sParserDV = new bReader(new DataView(await saveFile.arrayBuffer()), lEndian);

    let decompressedSize: number = 0;
    try {
        if (decompressZlib(sParserDV.slice(8))) {
            
            decompressedSize = Number(sParserDV.readULong(lEndian));
            if ((decompressedSize ?? 0) !== 0) {
                sParserDV = new bReader(new DataView(decompressZlib(sParserDV.slice(8)).buffer), lEndian);
            }
        }
    } catch (e) {
        console.log("ZLib decompression failed, file probably isnt compressed.");
    }

    /** Where the index is located in the file */
    const indexOffset = sParserDV.readUInt(lEndian);

    /** How many files are located in the index */
    const indexCount = sParserDV.readUInt(lEndian);

    /** Unknown Int, might be BOM.
     *  0A 00 0A 00 (LE)
     *  00 0B 00 0B (BE)
    */ 
    const BOM = sParserDV.readUInt(lEndian);
    sParserDV.incrementPos(indexOffset - 12);

    for (var i = 0; i < indexCount; i++) {
        while (sParserDV.getPos() + 144 <= sParserDV.byteLength()) {
            const bytes = sParserDV.slice(sParserDV.getPos(), sParserDV.getPos() + 144);

            /** Name of the file in the index */
            const fileName = new TextDecoder(lEndian === true ? 'utf-16le' : 'utf-16be').decode(bytes.slice(0, 128)).replace(/\0+$/, '');
            sParserDV.incrementPos(128);
            const fileLength = sParserDV.readUInt(lEndian);
            const fileOffset = sParserDV.readUInt(lEndian);
            const fileTimestamp = Number(sParserDV.readULong(lEndian));
            const fileData: ArrayBuffer = sParserDV.slice(fileOffset, fileOffset + fileLength);
            save.push({"name": fileName, "length": fileLength, "offset": fileOffset, "timestamp": fileTimestamp, "data": new File( [new Blob( [ new Uint8Array(fileData) ] )], fileName )})
        }
    }

    return save;
}