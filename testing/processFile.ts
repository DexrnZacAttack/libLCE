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

import * as fs from 'fs';
import * as path from 'path';
import { decompressSwitchPS4RLE, readSave } from '../src/index.js';
import { readRegion } from '../src/region/readRegion.js';

async function processFile(file: File): Promise<void> {
    switch (process.argv[3]) {
        case "-d":
            switch (process.argv[4]) {
            case "sprle":
                fs.writeFile(process.argv[2] + "_unsprle", Buffer.from(decompressSwitchPS4RLE(new Uint8Array(await file.arrayBuffer()))!), (err) => {
                });
                break;
            }
            break;
        case "-r":
            switch (process.argv[4]) {
                case "save":
                    console.log(await readSave(file));
                    break;
                case "region":
                    const region = await readRegion(file);
                    // fs.writeFile(process.argv[2] + "_regionReadTest", Buffer.from(region!), () => {
                    // });
                    break;
            }
            break;
        default:
            break;  
    }
}

function readFile(filePath: string, callback: (file: File) => void): void {
    fs.readFile(filePath, (err, data) => {
        callback(new File([data], path.basename(filePath)));
    });
}

const filePath = process.argv[2];
if (!filePath) {
    throw new Error("no path provided");
}

readFile(filePath, processFile);
