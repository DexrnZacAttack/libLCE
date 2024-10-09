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

import { getImageOffset } from "../console/consoles.js";
import { ConsoleTypes, World, WorldInfo } from "../index.js";
import { bReader } from "binaryio.js";
import extractChunks from "png-chunks-extract";

// TODO: make writer
export function readWorldInfo(data: Uint8Array, consoleType: ConsoleTypes): World {
    let reader = new bReader(data);
    const imageOffset = getImageOffset(consoleType);

    let worldName = "Not in file";

    if (imageOffset !== 0) {
        worldName = reader.readNullTerminatedString16();
        reader.setPos(imageOffset);
    }

    const imageData = new Uint8Array(reader.read(reader.byteLength - imageOffset));
    const tEXt = extractChunks(imageData).filter(chunk => chunk.name === 'tEXt').pop()?.data! // get last tEXt chunk

    const kv = [];
    const wInfoReader = new bReader(tEXt);

    while (wInfoReader.pos < wInfoReader.byteLength) {
        const tKv = wInfoReader.readNullTerminatedString8();
        if (tKv != "")
            kv.push(tKv);
    }

    const worldInfo: Partial<WorldInfo> = {};

    for (let i = 0; i < kv.length; i += 2) {
        const key = kv[i] as keyof WorldInfo;
        const value = kv[i + 1];
        
        worldInfo[key] = value;
    }


    return {name: worldName, thumbnail: imageData, worldInfo: worldInfo as WorldInfo};
}