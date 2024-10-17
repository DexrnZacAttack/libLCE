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

import { bReader } from "binaryio.js";
import { Keys, LangId, Language, Loc } from "../index.js";

export async function readLOC(locFile: File): Promise<Loc> {

    let reader = new bReader(await locFile.arrayBuffer());

    const version = reader.readUInt();
    const count = reader.readUInt();

    const keys: Keys = {useUniqueIDs: false, count: 0, keys: []};
    const languageIds: LangId[] = [];
    const languages: Language[] = [];

    if (version === 2) {
        keys.useUniqueIDs = reader.readByte() == 1;
        keys.count = reader.readUInt();
        for (var i = 0; i < keys.count; i++)
            keys.keys.push(reader.readUInt());
    }

    for (var i = 0; i < count; i++) {
        languageIds.push({name: reader.readString8(), unk: reader.readUInt()});
    }

    for (var i = 0; i < count; i++) {
        let byte;
        const shouldReadByte = reader.readUInt();
        if (shouldReadByte > 1)
            byte = reader.readByte(); 
        const langName = reader.readString8();
        const stringCount = reader.readUInt();
        let strings: string[] = [];
        for (var s = 0; s < stringCount; s++)
            strings.push(reader.readString8());

        languages.push({language: langName, stringCount, strings, shouldReadByte, byte});
    }

    return {version, count, keys, languageIds, languages}
}