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

import { World } from "../index.js";

export async function parseWorldInfo(file: File): Promise<World> {
    /** DV of file that was put in */
    let worldInfoDV = new DataView(await file.arrayBuffer());

    if (worldInfoDV.byteLength == 0)
        throw new Error(`File is of bad length ${worldInfoDV.byteLength}`);

    /** header of the world (includes name and a few other things) */
    const worldHeader = worldInfoDV.buffer.slice(0, 255);
    /** length of world name (from byte 0 till 2 null bytes are encountered) */
    var worldNameLength = 0;
    for (let i = 0; i < worldHeader.byteLength; i++) {
        if (worldInfoDV.getUint8(i) === 0x00 && worldInfoDV.getUint8(i + 1) === 0x00) {
            worldNameLength = i;
            break;
        }
    }

    /** the world name */
    const worldName = new TextDecoder('utf-16be').decode(worldInfoDV.buffer.slice(0, worldNameLength)).replace(/\0+$/, '');

    /** the world thumbnail */
    const thumbnail = new File([new Blob([worldInfoDV.buffer.slice(255, worldInfoDV.byteLength)])], "thumbnail.png");

    return {"name": worldName, "thumbnail": thumbnail};
}