import { compressionTypes, compressRLE, compressVitaRLE, compressZlib } from "../index.js";

export async function compressSave(save: File, compType: compressionTypes, lEndian = false): Promise<File> {
    // arraybuffer variable
    const fileArray = await save.arrayBuffer();
    const fileBuffer = Buffer.from(fileArray);
    /** decompressed size as bigint */
    const fileSize = BigInt(fileArray.byteLength)
    /** keep track of current position in stream */
    let currentOffset = 0;

    let compressedFile = new Uint8Array(fileArray);

    switch (compType) {
        case compressionTypes.zlib:
            compressedFile = compressZlib(fileArray);
            break;
        case compressionTypes.rle:
            compressedFile = compressRLE(fileArray);
            break;
        case compressionTypes.vitarle:
        case compressionTypes.switchrle:
        case compressionTypes.lzx:
        case compressionTypes.gzip:
        default:
            break;
        case compressionTypes.none:
            return new File([new Blob([fileArray])], 'savegame.dat');
    }

    /** allocate a dataview for the compressed data */
    const comp = new DataView(Buffer.alloc(8 + compressedFile.length, fileBuffer).buffer, 0, compressedFile.length + 8);
    comp.setBigUint64(0, fileSize, lEndian)
    currentOffset += 8;

    for (var i: number = 0; i < compressedFile.length - 1; i++) {
        comp.setUint8(currentOffset, compressedFile[i]);
        currentOffset += 1;
    }

    return new File([new Blob([comp.buffer])], 'savegame.dat');
}