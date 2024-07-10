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

import { msscmpFile } from "../index.js";
import { bReader } from "../io/bReader.js";

interface offsetIndex {
    filePathNameOffset: number,
    fileInfoOffset: number
}

interface fileIndex {
    fileNameOffset: number,
    fileDataOffset: number, // for some reason it seems to always be little endian
    sampleRate: number,
    fileSize: number
}

export async function readMSSCMP(file: File, lEndian?: boolean): Promise<msscmpFile[]> {

    const files: msscmpFile[] = [];
    const fIndex: fileIndex[] = [];
    const oIndex: offsetIndex[] = [];
    const reader = new bReader(new DataView(await file.arrayBuffer()), lEndian);

    const magic = reader.readString8(4);
    if (magic == "BANK" || magic == "KNAB") {
        if (typeof lEndian === 'undefined') {
            // if lEndian wasn't defined during fn call then it will use magic for endianness.
            lEndian = (magic == "KNAB");
            reader.setEndianness(lEndian);
        }
    } else {
        throw new Error(`Invalid file magic! (expected magic "BANK" or "KNAB", got ${magic} instead)`);
    }

    reader.setPos(0x18);
    const indexOffset = reader.readUInt();
    reader.setPos(0x34);
    const fileCount = reader.readUInt();

    reader.setPos(indexOffset);

    for (let i = 0; i < fileCount; i++) {
        const filePathNameOffset = reader.readUInt();
        const fileInfoOffset = reader.readUInt();
        oIndex.push({filePathNameOffset, fileInfoOffset});
    }


    oIndex.forEach(indexEntry => {
        reader.setPos(indexEntry.fileInfoOffset);
        reader.readUInt(); // seems to be path name offset... twice?
        const fileNameOffset = reader.readUInt();
        const fileDataOffset = reader.readUInt(true);
        reader.readULong(); // idek what this does... maybe 2 int32s or 1 int64, but it's usually 0x00000001FFFFFFFF or 0x00000002FFFFFFFF
        const sampleRate = reader.readUInt();
        const fileSize = reader.readUInt();
        fIndex.push({fileNameOffset, fileDataOffset, sampleRate, fileSize});
    });

    for (let i = 0; i < oIndex.length && i < fIndex.length; i++) {
        const oIndexEntry = oIndex[i];
        const fIndexEntry = fIndex[i];
    
        reader.setPos(oIndexEntry!.filePathNameOffset);
        const filePath = reader.readNullTerminatedString8();
        
        reader.setPos(oIndexEntry!.fileInfoOffset + fIndexEntry!.fileNameOffset);
        const fileName = reader.readNullTerminatedString8();

        const fileData = new Uint8Array(fIndexEntry!.fileSize);
        reader.setPos(fIndexEntry!.fileDataOffset);
        for (let curPos = 0; curPos < fIndexEntry!.fileSize; curPos++) {
            fileData[curPos] = reader.readByte();
        }

        const fullFileName = filePath + "/" + fileName.substring(1 + fIndexEntry!.fileSize.toString().length + 1);

        files.push({
            fileName: fullFileName,
            fileSize: fIndexEntry!.fileSize,
            sampleRate: fIndexEntry!.sampleRate,
            file: new File([fileData], fullFileName)
        });
    }

    return files;

} 