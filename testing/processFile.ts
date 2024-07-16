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
