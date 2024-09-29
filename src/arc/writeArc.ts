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

import { bWriter, bTypes } from "binaryio.js";

export async function writeArc(files: File[]): Promise<File> {
    /** Size of the index */
    let indexSize = 0;
    /** Size of all the files */
    let filesSize = 0;
    /** Total ARC file size */
    let totalSize = 0;

    files.forEach(file => {
        if (file.name.length > bTypes.USHORT_MAX_VALUE)
            throw new RangeError(`File name (${file.name}) would exceed ${bTypes.USHORT_MAX_VALUE} bytes in size, which is bigger than a unsigned short can store.`);

        indexSize += 2 + file.name.length + 4 + 4;
        filesSize += file.size;
    });

    totalSize = 4 + indexSize + filesSize;

    if (totalSize > bTypes.UINT_MAX_VALUE)
        throw new RangeError(`File size (${totalSize}) would exceed ${bTypes.UINT_MAX_VALUE}, which is bigger than an unsigned int can store.`);

    const writer = new bWriter(new ArrayBuffer(totalSize))

    // Write file count
    writer.writeUInt(files.length);

    /** Offset of the last written entry */
    let lastEntryOffset = 0;
    /** Where the file should be written */
    let filePointer = indexSize + 4;
    /** How many files we've written so far */
    let filesWritten = 1;

    for (const file of files) {
        // Write the size of the file name 
        writer.writeUShort(file.name.length);
        // Write the file name
        writer.writeString8(file.name, false);
        // Write the offset
        writer.writeUInt(filePointer);
        // Write the length
        writer.writeUInt(file.size); 

        // Get offset of the last entry
        lastEntryOffset = writer.pos;

        // Set our position to where we want the file to be written
        writer.setPos(filePointer);

        // Write the file
        writer.write(new Uint8Array(await file.arrayBuffer()));

        console.log(`Wrote ${file.name}`);

        // Go back to the last entry to write the next file
        writer.setPos(lastEntryOffset);

        filesWritten++;
        filePointer += file.size;
    }

    return new File([writer.arrayBuffer], "file.arc");
}