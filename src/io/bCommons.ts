/*
 * Copyright (c) 2024 DexrnZacAttack
 * This file is part of libLCE.
 * https://github.com/DexrnZacAttack/libLCE
 *
 * File Contributors (based off of GitHub commits):
 * - DexrnZacAttack
 *
 * Licensed under the MIT License. See LICENSE file for details.
*/

export class bCommons {
    protected dv: DataView;
    protected isLittle: boolean;
    protected curPos: number;

    constructor(dv: DataView, isLittle: boolean = false, curPos = 0) {
        this.dv = dv;
        this.isLittle = isLittle;
        this.curPos = curPos;
    }

    /** Returns the ArrayBuffer of the entire stream. */
    public get buffer(): ArrayBuffer {
        return this.dv.buffer;
    }

    /** Equivalent to ArrayBuffer.byteLength */
    public get byteLength(): number {
        return this.dv.buffer.byteLength;
    }

    /** Equivalent to ArrayBuffer.slice
     * @param start Where to begin
     * @param end Where to end
    */
    public slice(start: number, end?: number): ArrayBuffer {
        return this.dv.buffer.slice(start, end);
    }

    /** Increments the position in the stream
     * @param num How much to increment the position
    */
    public incrementPos(num: number): void {
        this.curPos += num;
    }

    /** Decrements the position in the stream
     * @param num How much to decrement the position
    */
    public decrementPos(num: number): void {
        this.curPos -= num;
    }

    /** Sets the position in the string
     * @param num What position to be at
    */
    public setPos(num: number): void {
        this.curPos = num;
    }

    /** Returns the current position in the stream */
    public get pos(): number {
        return this.curPos;
    }

    /** Sets the endianness
     * @param isLittleEndian If true, it gets set to Little Endian.
    */
    public setEndianness(isLittleEndian: boolean): void {
        this.isLittle = isLittleEndian;
    }
}