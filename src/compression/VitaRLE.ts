/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 * 
 * Other credits:
 * https://github.com/zugebot/LegacyEditor
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

import {bReader, bWriter} from "binaryio.js";

/**
 * The VitaRLE code is Zugebot (jerrinth3glitch)'s code ported to TS.
 * https://github.com/zugebot/LegacyEditor
 *
 * Big thanks to Offroaders for helping out with this, would've been barely possible without them!
*/

/**
 * Decompresses VitaRLE (used on PSVita Edition).
 * @param data The compressed data
 * @returns The decompressed data
*/
// Note: This is ported from LegacyEditor's rle_vita.cpp
export function decompressVitaRLE(data: Uint8Array): Uint8Array {
  // TODO: use bReader bWriter once they are dynamically sizable
  const compressedLength = data.byteLength;
  const result: number[] = [];
  let readOffset = 0;
  let writeOffset = 0;

  while (readOffset < compressedLength) {
    const suspectedTag: number = data[readOffset]!;
    readOffset++;

    if (suspectedTag !== 0) {
      result[writeOffset] = suspectedTag;
      writeOffset++;
    } else {
      const length: number = data[readOffset]!;
      readOffset++;
      for (let i = 0; i < length; i++) {
        result.push(0);
        writeOffset++;
      }
    }
  }

  return new Uint8Array(result);
}

// TODO: remove these getSize functions, they are janky.
function getCompressedSize(data: Uint8Array): number {
  const sizeIn = data.byteLength;
  const reader = new bReader(data);
  let zeroCount: number = 0;
  let compressedSize: number = 0;

  for (var i = 0; i < sizeIn; ++i) {
    if (reader.readByte() != 0) {
      if (zeroCount > 0) {
        compressedSize += 2;
        zeroCount = 0;
      }
      compressedSize += 1;
    } else {
      zeroCount++;
      if (zeroCount == 255 || i == sizeIn - 1) {
        compressedSize += 2;
        zeroCount = 0;
      }
    }
  }

  if (zeroCount > 0) {
    compressedSize += 2;
  }

  return compressedSize;
}



/**
 * Compresses with VitaRLE (used on PSVita Edition).
 * @param data Data to compress
 * @returns Compressed data
 */
// Note: This is ported from LegacyEditor's rle_vita.cpp
export function compressVitaRLE(data: Uint8Array): Uint8Array {

  const writer = new bWriter(new ArrayBuffer(getCompressedSize(data)));
  const reader = new bReader(data);

  let zeroCount: number = 0;

  for (let i = 0; i < data.byteLength; ++i) {
    let value: number;
    if ((value = reader.readByte()) != 0x00) {
      if (zeroCount > 0) {
        writer.writeByte(0x00);
        writer.writeByte(zeroCount);
        zeroCount = 0;
      }
      writer.writeByte(value);
    } else {
      zeroCount++;
      if (zeroCount == 255 || i == data.byteLength - 1) {
        writer.writeByte(0x00);
        writer.writeByte(zeroCount);
        zeroCount = 0;
      }
    }
  }

  if (zeroCount > 0) {
    writer.writeByte(0x00);
    writer.writeByte(zeroCount);
  }

  return new Uint8Array(writer.arrayBuffer);
}
