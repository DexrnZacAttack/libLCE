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

// Dexrn: Scuffed BinaryReader clone.
export class bReader {
    // TODO: use sizable arrays instead of allocated at call time
    dvRead: DataView;
    curPos: number;
    isLittle: boolean;

    /** Creates an endianness-aware DataView with built in position tracking
     * @param dvRead The DataView
     * @param isLittle Whether or not to read as Little Endian
     * @param curPos Position to start at
    */
    constructor(dvRead: DataView, isLittle: boolean = false, curPos = 0) {
        this.dvRead = dvRead;
        this.isLittle = isLittle;
        this.curPos = curPos;
    }

    /** Returns the ArrayBuffer of the entire stream. */
    getBuffer(): ArrayBuffer {
        return this.dvRead.buffer;
    }

    /** Equivalent to ArrayBuffer.byteLength */
    byteLength(): number {
        return this.dvRead.buffer.byteLength;
    }

    /** Equivalent to ArrayBuffer.slice
     * @param start Where to begin
     * @param end Where to end
    */
    slice(start: number, end: number | undefined = undefined): ArrayBuffer {
        return this.dvRead.buffer.slice(start, end);
    }

    /** Reads {size} amount of bytes
     * @param size How many bytes to read
     * @returns The read bytes in an ArrayBuffer (Uint8Array)
     */
    read(size: number): ArrayBuffer {
        let ab: Uint8Array = new Uint8Array(size);
        for (let i = 0; i < size; i++) {
            ab[i] = this.readByte();
        }
        return ab;
    }

    /** Reads a Byte */
    readByte(): number {
        return this.dvRead.getUint8(this.curPos++);
    }

    /** Reads an SByte */
    readSByte(): number {
        return this.dvRead.getInt8(this.curPos++);
    }

    /** Reads a UShort
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readUShort(isLittleEndian = this.isLittle): number {
        const ushort = this.dvRead.getUint16(this.curPos, isLittleEndian);
        this.curPos += 2;
        return ushort;
    }

    /** Reads a Short
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readShort(isLittleEndian = this.isLittle): number {
        const short = this.dvRead.getInt16(this.curPos, isLittleEndian);
        this.curPos += 2;
        return short;
    }

    /** Reads a UInt
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readUInt(isLittleEndian = this.isLittle): number {
        const uint = this.dvRead.getUint32(this.curPos, isLittleEndian);
        console.log(this.curPos);
        this.curPos += 4;
        return uint;
    }

    /** Reads an Int
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readInt(isLittleEndian = this.isLittle): number {
        const int = this.dvRead.getInt32(this.curPos, isLittleEndian);
        this.curPos += 4;
        return int;
    }

    /** Reads a ULong
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readULong(isLittleEndian = this.isLittle): bigint {
        const ulong = this.dvRead.getBigUint64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return ulong;
    }

    /** Reads a Long
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readLong(isLittleEndian = this.isLittle): bigint {
        const long = this.dvRead.getBigInt64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return long;
    }

    /** Reads a Float
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readFloat(isLittleEndian = this.isLittle): number {
        const float = this.dvRead.getFloat32(this.curPos, isLittleEndian);
        this.curPos += 4;
        return float;
    }

    /** Reads a Double
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readDouble(isLittleEndian = this.isLittle): number {
        const double = this.dvRead.getFloat64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return double;
    }

    /** Reads a UTF16 string
     * @param Length length of the string in bytes, including null bytes
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    readString16(length: number = this.readUShort(), isLittleEndian = this.isLittle): string {
        let str = "";
        for (var i = 0; i < (length / 2); i++) {
            str += String.fromCharCode(this.readUShort(isLittleEndian));
        }
        return str.replace(/\x00/g, '');
    }

    /** Reads a UTF8 string
     * @param length Length of the string in bytes
    */
    readString8(length: number = this.readByte()): string {
        let str = "";
        for (var i = 0; i < length; i++) {
            str += String.fromCharCode(this.readByte());
        }
        return str;
    }

    /** Increments the position in the stream
     * @param num How much to increment the position
    */
    incrementPos(num: number): void {
        this.curPos += num;
    }

    /** Decrements the position in the stream
     * @param num How much to decrement the position
    */
    decrementPos(num: number): void {
        this.curPos -= num;
    }

    /** Returns the current position in the stream */
    getPos(): number {
        return this.curPos;
    }

    /** Reads a Byte without incrementing the position in the stream
     * @param offset Offset to read from
    */
    peekByte(offset: number = this.curPos): number {
        return this.dvRead.getUint8(offset);
    }

    /** Reads an SByte without incrementing the position in the stream
     * @param offset Offset to read from
    */
    peekSByte(offset: number = this.curPos): number {
        return this.dvRead.getInt8(offset);
    }

    /** Reads a UShort without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekUShort(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dvRead.getUint16(offset, isLittleEndian);
    }

    /** Reads a Short without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekShort(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dvRead.getInt16(offset, isLittleEndian);
    }

    /** Reads a UInt without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekUInt(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dvRead.getUint32(offset, isLittleEndian);
    }

    /** Reads an Int without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekInt(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dvRead.getInt32(offset, isLittleEndian);
    }

    /** Reads a ULong without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekULong(offset: number = this.curPos, isLittleEndian = this.isLittle): bigint {
        return this.dvRead.getBigUint64(offset, isLittleEndian);
    }

    /** Reads a Long without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekLong(offset: number = this.curPos, isLittleEndian = this.isLittle): bigint {
        return this.dvRead.getBigInt64(offset, isLittleEndian);
    }

    /** Reads a Float without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekFloat(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dvRead.getFloat32(offset, isLittleEndian);
    }

    /** Reads a Double without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekDouble(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dvRead.getFloat64(offset, isLittleEndian);
    }

    /** Reads a UTF16 string without incrementing the position in the stream
     * @param length length of the string in bytes, including null bytes
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    peekString16(length: number = this.readByte(), offset: number = this.curPos, isLittleEndian = this.isLittle): string {
        let str = "";
        for (var i = 0; i < length; i++) {
            str += (String.fromCharCode(this.peekUShort(offset + (i + 1), isLittleEndian)));
        }
        return str;
    }

    /** Reads a UTF8 string without incrementing the position in the stream
     * @param length Length of the string in bytes
     * @param offset Offset to read from
    */
    peekString8(length: number = this.readByte(), offset: number = this.curPos) {
        let str = "";
        for (var i = 0; i < length; i++) {
            str += (String.fromCharCode(this.peekByte(offset + i)));
        }
        return str;
    }
}