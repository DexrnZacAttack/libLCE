import { readdir, readFile, stat, writeFile } from "fs/promises";

import path, { join, resolve, relative } from "path";

import { CompressionTypes, compressSave, writeSave, readARC, readSave, writeArc, readMSSCMP } from "../src/index.js"; 
import { existsSync, lstatSync, mkdirSync, readdirSync, readFileSync, statSync } from "fs";

async function runArcTest(): Promise<ArrayBuffer> {
    try {
        return (await (await writeArc(await getFiles())).arrayBuffer());
    } catch (e) {
        throw new Error(`Error reading arc: ${e}`);
    }
}

async function getFiles(): Promise<File[]> {
    const path2Folder: string = resolve("./example");
    const files: File[] = await Promise.all(
        (
            await readdir(path2Folder, { recursive: true, withFileTypes: true })
        )
            .filter((dirent) => dirent.isFile())
            .map(async (dirent) => {
                const path: string = join(dirent.parentPath, dirent.name);
                const data: Buffer = await readFile(path);
                const { mtimeMs } = await stat(path);
                return new File([data], relative(path2Folder, path), { lastModified: mtimeMs });
            })
    );

    return files;
}

async function runSaveTest(): Promise<ArrayBuffer> {
    try {
        return (await (await writeSave(await getFiles(), false)).arrayBuffer());
    } catch (e) {
        throw new Error(`Error reading save: ${e}`);
    }
}

async function runMSSCMPTest() {
    try {
        const files = readdirSync("reading\\MSSCMP");

        for (const file of files) {
            const filePath = path.join("reading\\MSSCMP", file);
            if (statSync(filePath).isFile()) {
                console.log(`Reading MSSCMP ${filePath}`)
                await readMSSCMP(new File([readFileSync(filePath)], "msscmptest"));
            }
        }
    } catch (error) {
        console.error('Error reading MSSCMP:', error);
    }
}


async function runTests() {
    if (!existsSync("results")) {
        mkdirSync("results");
    }

    console.log("Generating uncompressed ARC");
    writeFile("results/example_arc.arc", new DataView(await runArcTest()));

    console.log("Generating uncompressed save");
    writeFile("results/example_save_uncompressed.dat", new DataView(await runSaveTest()));
    
    console.log("Generating Zlib-compressed save");
    writeFile("results/example_save_zlib.dat", new DataView(compressSave(await runSaveTest(), CompressionTypes.zlib).buffer));

    console.log("Generating Vita RLE-compressed save");
    writeFile("results/example_save_vitarle.dat", new DataView(compressSave(await runSaveTest(), CompressionTypes.vitarle).buffer));

    console.log("Generating Switch/PS4 RLE-compressed save");
    writeFile("results/example_save_switchPs4Rle.dat", new DataView(compressSave(await runSaveTest(), CompressionTypes.switchps4rle).buffer));

    console.log("Reading uncompressed save");
    console.log(await readSave(new File([await readFile("results/example_save_uncompressed.dat")], "example_save_uncompressed.dat")));
    console.log("Reading ZLib save");
    console.log(await readSave(new File([await readFile("results/example_save_zlib.dat")], "example_save_zlib.dat")));
    // do these later
    // console.log(await readSave(new File([await readFile("results/example_save_vitarle.dat")], "example_save_vitarle.dat")));
    // console.log(await readSave(new File([await readFile("results/example_save_switchPs4Rle.dat")], "example_save_switchPs4Rle.dat")));

    console.log("Reading generated arc");
    console.log(await readARC(new File([await readFile("results/example_arc.arc")], "example_arc.arc")));

    // To run this test, place .MSSCMP files "./reading/MSSCMP".
    if (existsSync("reading/MSSCMP") && lstatSync("reading/MSSCMP").isDirectory()) {
        console.log("Reading MSSCMPs");
        runMSSCMPTest();
    }
}



runTests();