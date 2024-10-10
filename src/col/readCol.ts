import { bReader, RGBFormat, RGBColor } from "binaryio.js";
import { Col, ColColor, ColWorldColor } from "../index.js";

export function readCOL(col: Uint8Array): Col {
    const reader = new bReader(col);

    const version = reader.readUInt();

    const colorCount = reader.readUInt();
    const colors: ColColor[] = [];

    for (var i = 0; i < colorCount; i++) {
        const name = reader.readString8();
        const color = reader.readRGB(RGBFormat.ARGB);

        colors.push({name, color});
    }

    if (version > 0) {
        const worldColorCount = reader.readUInt();
        const worldColors: ColWorldColor[] = [];
    
        for (var i = 0; i < worldColorCount; i++) {
            const name = reader.readString8();
            const waterColor = reader.readRGB(RGBFormat.ARGB);
            const underwaterColor = reader.readRGB(RGBFormat.ARGB);
            const fogColor = reader.readRGB(RGBFormat.ARGB);
    
            worldColors.push({name, waterColor, underwaterColor, fogColor});
        }
        return({version, colors, worldColors: worldColors});
    }

    return({version, colors});
}