/**
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
export function decompressVitaRLE(data: Uint8Array): Uint8Array {
    const compressedLength = data.byteLength;
    const result: number[] = [];
    let readOffset = 0;
    let writeOffset = 0;
  
    while (readOffset < compressedLength){
      const suspectedTag: number = data[readOffset]!;
      readOffset++;
  
      if (suspectedTag !== 0){
        result[writeOffset] = suspectedTag;
        writeOffset++;
      } else {
        const length: number = data[readOffset]!;
        readOffset++;
        for (let i = 0; i < length; i++){
          result.push(0);
          writeOffset++;
        }
      }
    }
  
    return new Uint8Array(result);
}

/**
 * Compresses with VitaRLE (used on PSVita Edition).
 * @param data The decompressed data
 * @returns The compressed data
*/
export function compressVitaRLE(data: Uint8Array): Uint8Array {
  const compressedLength = data.byteLength;
  const result: number[] = [];
  let readOffset = 0;
  let writeOffset = 0;
  let zeroCount = 0;

  while (readOffset < compressedLength){
    const suspectedTag: number = data[readOffset]!;
    readOffset++;

    if (suspectedTag !== 0){
      if (zeroCount > 0) {
        result[writeOffset] = 0;
        writeOffset++;
        result[writeOffset] = zeroCount;
        writeOffset++;
        zeroCount = 0;
      }
      result[writeOffset] = suspectedTag;
      writeOffset++;
    } else {
      const length: number = data[readOffset]!;
      readOffset++;
      for (let i = 0; i < length; i++){
        result.push(0);
        writeOffset++;
        zeroCount++;
        if (zeroCount === 255 || readOffset === compressedLength - 1) {
          result[writeOffset] = 0;
          writeOffset++;
          result[writeOffset] = zeroCount;
          writeOffset++;
          zeroCount = 0;
        }
      }
    }
  }

  if (zeroCount > 0) {
    result[writeOffset] = 0;
    writeOffset++;
    result[writeOffset] = zeroCount;
  }

  return new Uint8Array(result);
}
