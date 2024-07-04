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

import { bReader } from "../io/bReader.js";
import { bWriter } from "../io/bWriter.js";

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
  const reader = new bReader(new DataView(data.buffer));
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

  const writer = new bWriter(new DataView(new ArrayBuffer(getCompressedSize(data))));
  const reader = new bReader(new DataView(data.buffer));

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

  return new Uint8Array(writer.getBuffer());
}
