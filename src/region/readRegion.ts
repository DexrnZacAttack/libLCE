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

// https://modifiedcommand.github.io/Legacy-Docs/Saves/ChunkData/

import { decompressZlib } from "../index.js";
import { bReader } from "../io/bReader.js";

interface indice {
    offset: number,
    length: number
}

interface chunkData {
    isRLECompressed: boolean;
    unkBit2: boolean;
    compressedSize: number;
    decompressedSize: number;
    chunks?: chunk[];
};

interface chunk {
    version: number, // short
    xPos: number, // int
    zPos: number, // int
    lastUpdate: bigint,
    inhabitedTime: bigint,
    blockDataSize: number // short
    jumpTable: number[],
    sectionSizes: number[]
};

interface gridIndice {
    Y: number,
    X: number,
    Z: number
}

// broken.
function readAquaticChunk(data: Uint8Array, blockDataSize: number, jumpTable: number[], sectionSizes: number[]) {
    const reader = new bReader(new DataView(data.buffer), false);

    const sectionOffset = 0x4C + jumpTable[0]!;
    console.log(sectionOffset);

    const gridIndices: gridIndice[] = [];
    for (let i = 0!; i < 0x7F; i += 3) {
        reader.setPos(sectionOffset + i);
        console.log("0x" + reader.getPos().toString(16))
        const Y = reader.readByte();
        const X = reader.readByte();
        const Z = reader.readByte();

        gridIndices.push({ Y, X, Z });
    }

    console.log(gridIndices);

}

export function readChunkInfo(data: Uint8Array): chunk {
    const reader = new bReader(new DataView(data.buffer));
    const version: number = reader.readUShort();
    const xPos: number = reader.readUInt();
    const zPos: number = reader.readUInt();
    const lastUpdate: bigint = reader.readULong();
    const inhabitedTime: bigint = reader.readULong();
    const blockDataSize: number = (reader.readUShort() * 0x100); // THIS IS X
    const jumpTable: number[] = []; // THIS IS Y
    const sectionSizes: number[] = []; // THIS IS Z
    for (let timesRead = 0; timesRead < 16; timesRead++) {
        jumpTable.push(reader.readUShort());
    }
    for (let timesRead = 0; timesRead < 0x10; timesRead++) {
        sectionSizes.push(reader.readByte());
    }
    console.log("0x" + reader.getPos().toString(16))
    console.log(readAquaticChunk(data, blockDataSize, jumpTable, sectionSizes));
    console.log(reader.getPos());
    return({version, xPos, zPos, lastUpdate, inhabitedTime, blockDataSize, jumpTable, sectionSizes})
}

export async function readRegion(file: File) {
    const reader = new bReader(new DataView(await file.arrayBuffer()));
    const indices: indice[] = [];

    for (let pos = 0; pos < 0x0FFF; pos += 4) {
        const offset = reader.readUInt24();
        const length = reader.readByte();
        indices.push({offset, length});
    }
    console.log(reader.getPos());
    
    const timestamps: Array<number> = [];

    for (let pos = reader.getPos(); pos < 0x1FFF; pos += 4) {
        const ts = reader.readUInt();
        timestamps.push(ts);
    }

    console.log(timestamps);
    console.log(reader.getPos());

    const compressedSize = reader.readUInt(); // includes flags
    const decompressedSize = reader.readUInt();
    let chunkData: chunkData = {
        isRLECompressed: (compressedSize >> 31) !== 0,
        unkBit2: (compressedSize >> 30) !== 0,
        compressedSize: compressedSize & 0x3FFFFFFF,
        decompressedSize: decompressedSize,
        chunks: []
    };

    chunkData.chunks?.push(readChunkInfo(decompressZlib(reader.read(compressedSize & 0x3FFFFFFF))));
    console.log(chunkData);


}