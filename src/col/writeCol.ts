import { bWriter, RGBFormat } from "binaryio.js";
import { Col } from "../index.js";

export function writeCOL(col: Col): ArrayBuffer {
    let byteLength = 8; // 4 for version 4 for color count
    let colorCount = 0;
    let worldColorCount = 0;

    col.colors.forEach(color => {
        byteLength += (color.name.length + 2) + 4;
        colorCount += 1;
    });

    if (col.worldColors != undefined) {
        byteLength += 4;
        col.worldColors.forEach(color => {
            byteLength += (color.name.length + 2) + 4 + 4 + 4;
            worldColorCount += 1;
        });
    }

    const writer = new bWriter(new ArrayBuffer(byteLength));

    writer.writeUInt(col.version);

    writer.writeUInt(col.colors.length);

    col.colors.forEach(color => {
        writer.writeString8(color.name, true);
        writer.writeRGB(color.color, RGBFormat.ARGB);
    });

    if (col.worldColors != undefined) {
        if (col.version < 1)
            console.warn(`This COL version ${col.version} does not normally support world colors, nothing will likely read them.`);

        writer.writeUInt(col.worldColors.length);

        col.worldColors.forEach(color => {
            writer.writeString8(color.name, true);
            writer.writeRGB(color.waterColor, RGBFormat.ARGB);
            writer.writeRGB(color.underwaterColor, RGBFormat.ARGB);
            writer.writeRGB(color.fogColor, RGBFormat.ARGB);
        });
    }

    return writer.arrayBuffer;
}