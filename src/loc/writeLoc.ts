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

import { bWriter } from "binaryio.js";
import { Loc } from "../index.js";

export function writeLoc(loc: Loc): Uint8Array {
    // I need to find a better way to do this
    let totalSize = 0;

    totalSize += 4; // version
    totalSize += 4; // count
    if (loc.version == 2) {
        totalSize += 1; // use unique IDs
        totalSize += 4; // key count
        for (var i = 0; i < loc.keys!.count; i++)
            totalSize += 4; // key
    }

    for (var lc = 0; lc < loc.count; lc++) {
        totalSize += loc.languageIds[lc]!.name.length + 2; // language id name
        totalSize += 4; // unk
    }

    for (var lang = 0; lang < loc.languages.length; lang++) {
        totalSize += 4;
        if (loc.languages[lang]?.shouldReadByte! > 0)
            totalSize += 1

        totalSize += loc.languages[lang]?.language.length! + 2; // language name

        totalSize += 4; // string count

        for (var sc = 0; sc < loc.languages[lang]?.stringCount!; sc++)
            totalSize += loc.languages[lang]?.strings[sc]!.length! + 2; // string
    }

    const writer = new bWriter(new ArrayBuffer(totalSize));

    // version
    writer.writeUInt(loc.version);
    // count
    writer.writeUInt(loc.count);

    if (loc.version === 2) {
        // whether or not to use the unique ids
        writer.writeByte(loc.keys!.useUniqueIDs ? 1 : 0);
        // id count
        writer.writeUInt(loc.keys!.count);
        // ids (keys)
        for (var i = 0; i < loc.keys!.count; i++)
            writer.writeUInt(loc.keys!.keys[i]!);
    }

    for (var lc = 0; lc < loc.count; lc++) {
        // write language id name
        writer.writeString8(loc.languageIds[lc]!.name, true);
        // write some identifier (idk what it does)
        writer.writeUInt(loc.languageIds[lc]!.unk);
    }

    for (var lang = 0; lang < loc.languages.length; lang++) {
        // have no idea what this does lmfao
        writer.writeUInt(loc.languages[lang]?.shouldReadByte!)
        if (loc.languages[lang]?.shouldReadByte! > 0)
            writer.writeByte(loc.languages[lang]?.byte!);

        // write language name
        writer.writeString8(loc.languages[lang]?.language!, true);

        // write the string count
        writer.writeUInt(loc.languages[lang]?.stringCount!);

        // write the strings
        for (var sc = 0; sc < loc.languages[lang]?.stringCount!; sc++)
            writer.writeString8(loc.languages[lang]?.strings[sc]!, true);
    }

    return new Uint8Array(writer.arrayBuffer);
}
