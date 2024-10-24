import { readdir, readFile, stat, writeFile } from "fs/promises";

import path, { join, resolve, relative } from "path";

import { CompressionTypes, compressSave, writeSave, readARC, readSave, writeArc, readMSSCMP, readPCK, writeCOL, readCOL, decompressSave } from "../src/index.js"; 
import { existsSync, lstatSync, mkdirSync, readdirSync, readFileSync, statSync } from "fs";
import { RGBColor } from "binaryio.js";

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

async function runPCKTest() {
    try {
        const files = readdirSync("reading\\PCK");

        for (const file of files) {
            const filePath = path.join("reading\\PCK", file);
            if (statSync(filePath).isFile()) {
                console.log(`Reading PCK ${filePath}`)
                await readPCK(new File([readFileSync(filePath)], "pcktest"));
            }
        }
    } catch (error) {
        console.error('Error reading PCK:', error);
    }
}


async function runTests() {
    if (!existsSync("results")) {
        mkdirSync("results");
    }

    console.log("Generating ARC");
    writeFile("results/example_arc.arc", new DataView(await runArcTest()));

    console.log("Generating COL");
    writeFile("results/example_col.col", new Uint8Array(writeCOL({version: 1,
        colors: [{name: "Hello, world!",
            color: new RGBColor(0x18, 0x34, 0xAD, 0xFF)}],
             worldColors: [{name: "libLCE COL world color writing test",
                waterColor: new RGBColor(0x18, 0x7A, 0x92, 0xFF),
                 underwaterColor: new RGBColor(0x33, 0x4E, 0xC4, 0xFF),
                  fogColor: new RGBColor(0x00, 0x41, 0x91, 0xFF)}]})));

    console.log("Generating uncompressed save");
    writeFile("results/example_save_uncompressed.dat", new DataView(await runSaveTest()));
    
    console.log("Generating Zlib-compressed save");
    writeFile("results/example_save_zlib.dat", new DataView(compressSave(await runSaveTest(), CompressionTypes.Zlib).buffer));

    console.log("Generating Vita RLE-compressed save");
    writeFile("results/example_save_vitarle.dat", new DataView(compressSave(await runSaveTest(), CompressionTypes.VitaRle).buffer));

    console.log("Generating Split Save RLE-compressed save");
    writeFile("results/example_save_splitsaverle.dat", new DataView(compressSave(await runSaveTest(), CompressionTypes.SplitSaveRle).buffer));

    console.log("Reading uncompressed save");
    console.log(await readSave(new File([await readFile("results/example_save_uncompressed.dat")], "example_save_uncompressed.dat")));
    console.log("Reading ZLib save");
    console.log(await readSave(new File([await readFile("results/example_save_zlib.dat")], "example_save_zlib.dat")));
    console.log("Reading Vita RLE-compressed save");
    console.log(await readSave(new File([decompressSave(await readFile("results/example_save_vitarle.dat"), CompressionTypes.VitaRle)], "example_save_vitarle.dat")));
    console.log("Reading Split Save RLE-compressed save");
    console.log(await readSave(new File([decompressSave(await readFile("results/example_save_splitsaverle.dat"), CompressionTypes.SplitSaveRle)], "example_save_zlib.dat")));

    console.log("Reading generated arc");
    console.log(await readARC(new File([await readFile("results/example_arc.arc")], "example_arc.arc")));

    console.log("Reading generated col");
    console.log(readCOL(new Uint8Array((await readFile("results/example_col.col")).buffer)));

    // To run this test, place .MSSCMP files "./reading/MSSCMP".
    if (existsSync("reading/MSSCMP") && lstatSync("reading/MSSCMP").isDirectory()) {
        console.log("Reading MSSCMPs");
        await runMSSCMPTest();
    }

    // To run this test, place .PCK files "./reading/PCK".
    if (existsSync("reading/PCK") && lstatSync("reading/PCK").isDirectory()) {
        console.log("Reading PCKs");
        await runPCKTest();
    }
}



runTests();