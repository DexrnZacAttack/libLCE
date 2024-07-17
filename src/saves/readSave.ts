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

import { decompressZlib, index, save, saveVersion } from "../index.js";
import { bReader } from "../io/bReader.js";

interface readOptions {
    /** Ignores the version parameter which is used for endianness detection. */
    ignoreVersion?: boolean
}

export async function readSave(saveFile: File, lEndian = false, ro: readOptions = {ignoreVersion: false}): Promise<save> {
    const index: index[] = []

    let saveReader = new bReader(new DataView(await saveFile.arrayBuffer()), lEndian);

    let decompressedSize: number = 0;
    try {
        if (decompressZlib(saveReader.slice(8))) {
            
            decompressedSize = Number(saveReader.readULong());
            if ((decompressedSize ?? 0) !== 0) {
                saveReader = new bReader(new DataView(decompressZlib(saveReader.slice(8)).buffer), lEndian);
            }
        }
    } catch (e) {
        console.log("ZLib decompression failed, file probably isnt compressed.");
    }

    if (!ro.ignoreVersion) {
        saveReader.setPos(10);
        const ver = saveReader.readUShort();
        // it works though...
        if (ver > 50) {
            console.warn(`Invalid version ${ver}, trying with reverse endian... To disable this behavior, you need to have ignoreVersion set to true in the ro param.`);
            saveReader.setEndianness(!lEndian);
        }
        saveReader.setPos(0);
    }

    /** Where the index is located in the file */
    const indexOffset = saveReader.readUInt();

    
    /** How many files are located in the index */
    let indexCount = saveReader.readUInt();

    // https://github.com/zugebot/legacyeditor for these 2 shorts
    /** Minimum supported LCE version */
    const fileMinimumVersion = saveReader.readUShort();
    /** Current LCE file version */
    const fileVersion = saveReader.readUShort();

    let indexEntrySize = 144;

    /** Determines whether or not the save file is ver 0-1 which has a slightly different format. */
    const isPreReleaseSF = fileVersion == saveVersion.TU0033;

    if (isPreReleaseSF) {
        // first 2 pr versions have count in bytes.
        indexEntrySize = 136;
        indexCount = indexCount / indexEntrySize;
    }
    
    saveReader.incrementPos(indexOffset - 12);

    for (var i = 0; i < indexCount; i++) {
        while (saveReader.pos + indexEntrySize <= saveReader.byteLength) {
            /** Name of the file in the index */
            const fileName = saveReader.readString16(128);
            /** Length of file in the index */
            const fileLength = saveReader.readUInt();
            /** Location (Offset) of the file in the index */
            const fileOffset = saveReader.readUInt();

            /** Timestamp of the file (unusable due to how it's written.) */
            let fileTimestamp = 0n;
            if (!isPreReleaseSF) {
                // first 2 pr versions don't include timestamp
                fileTimestamp = saveReader.readULong();
            }
            
            const fileData: ArrayBuffer = saveReader.slice(fileOffset, fileOffset + fileLength);
            index.push({"name": fileName, "length": fileLength, "offset": fileOffset, "timestamp": fileTimestamp, "data": new File( [new Blob( [ new Uint8Array(fileData) ] )], fileName )})
        }
    }

    return {"indexOffset": indexOffset, "fileCount": indexCount, "fileMinVerSupported": fileMinimumVersion, "fileVersion": fileVersion, "fileIndex": index};
}