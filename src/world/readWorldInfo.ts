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
import { consoleTypes, World, worldInfo } from "../index.js";
import { bReader } from "../io/bReader.js";
import * as png from '@vivaxy/png';

// TODO: fix this below
// TODO: make writer

function parse4JtEXt(data: {"wInfo": string}): worldInfo {
    // this is a bad way of doing it!!!!
    const textChunks = data.wInfo.split('\x00');
    const parsed: Record<string, string> = {};

    for (let i = 0; i < textChunks.length - 1; i += 2) {
        const key = textChunks[i]!.trim();
        const value = textChunks[i + 1]!.trim();
        parsed[key] = value;
    }
    
    return parsed as unknown as worldInfo;
}

export async function parseWorldInfo(file: File, console: consoleTypes): Promise<World> {
    let reader = new bReader(new DataView(await file.arrayBuffer()));
    const imageOffset = getImageOffset(console);

    let worldName;

    if (imageOffset !== 0) {
        /** header of the world (includes name and a few other things) */
        const worldHeader = new Uint8Array(reader.read(imageOffset));
        /** length of world name (from byte 0 till 2 null bytes are encountered) */
        var worldNameLength = 0;
        for (let i = 0; i < worldHeader.byteLength; i++) {
            if (worldHeader[i] === 0x00 && worldHeader[i + 1] === 0x00) {
                worldNameLength = i;
                break;
            }
        }
        /** the world name */
        worldName = new TextDecoder('utf-16be').decode(reader.slice(0, worldNameLength)).replace(/\0+$/, '');
    }

    /** Image data */
    const imageData = new Uint8Array(reader.read(reader.byteLength - imageOffset));

    // console.log(Array.from(imageData, byte => byte.toString(16).padStart(2, '0')).join(' '));
    const worldInfo = png.decode(imageData).text;
    // todo: FIX THIS EWWWWWW
    let worldInfoFixed = parse4JtEXt({'wInfo': Object.keys(worldInfo!).map(key => key + '\x00' + worldInfo![key]).join('')});

    /** the world thumbnail */
    const thumbnail = new File([new Blob([imageData])], "thumbnail.png", { type: "image/png" });

    return worldName ? {"name": worldName, "thumbnail": thumbnail, "worldInfo": worldInfoFixed} : {"thumbnail": thumbnail, "worldInfo": worldInfoFixed};
}