/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * 
 * Licensed under the MIT License. See LICENSE file for details.
*/

import { bReader } from 'binaryio.js';
import { arc, ArcIndex } from '../index.js';

export async function readARC(arc: File): Promise<arc> {
    const index: ArcIndex[] = []

    let arcReader = new bReader(await arc.arrayBuffer(), false);

    /** How many files are in the index */
    const indexCount = arcReader.readUInt();
    
    for (var i = 0; i < indexCount; i++) {
            /** Size of the file name */
            const fileNameSize = arcReader.readUShort();
            /** Name of the file */
            const fileName = arcReader.readString8(fileNameSize);
            /** Location (offset) of the file */
            const fileOffset = arcReader.readUInt();
            /** Length (size) of the file */
            const fileLength = arcReader.readUInt();

            const fileData: ArrayBuffer = arcReader.slice(fileOffset, fileOffset + fileLength);
            index.push({"name": fileName, "length": fileLength, "offset": fileOffset, "data": new File( [new Blob( [ fileData ] )], fileName )})
    }

    return {"fileCount": indexCount, "fileIndex": index};
}