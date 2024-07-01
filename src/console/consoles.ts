import { clientTypes, compressionTypes, endianness } from "../index.js";

export function getCompression(console: clientTypes) {
    const compTypesMap = new Map([
        [clientTypes.Xbox360, compressionTypes.lzx],
        [clientTypes.XboxOne, compressionTypes.zlib],
        [clientTypes.PS3, compressionTypes.rle],
        [clientTypes.Vita, compressionTypes.vitarle],
        [clientTypes.PS4, compressionTypes.switchrle],
        [clientTypes.WiiU, compressionTypes.zlib],
        [clientTypes.Switch, compressionTypes.switchrle]
    ])
    return compTypesMap.get(console) || compressionTypes.none;
}

export function getEndianness(console: clientTypes) {
    const endiannessMap = new Map([
        [clientTypes.Xbox360, endianness.big],
        [clientTypes.XboxOne, endianness.little],
        [clientTypes.PS3, endianness.big],
        [clientTypes.Vita, endianness.little],
        [clientTypes.PS4, endianness.little],
        [clientTypes.WiiU, endianness.big],
        [clientTypes.Switch, endianness.little]
    ])
    return endiannessMap.get(console) || endianness.big;
}