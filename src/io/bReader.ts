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

import { bCommons } from "./bCommons.js";

// Dexrn: Scuffed BinaryReader clone.
export class bReader extends bCommons {
    /** Creates an endianness-aware DataView with built in position tracking
     * @param dvRead The DataView
     * @param isLittle Whether or not to read as Little Endian
     * @param curPos Position to start at
    */
    constructor(dvRead: DataView, isLittle: boolean = false, curPos = 0) {
        super(dvRead, isLittle, curPos);
    }

    /** Reads {size} amount of bytes
     * @param size How many bytes to read
     * @returns The read bytes in an ArrayBuffer (Uint8Array)
    */
    public read(size: number): ArrayBuffer {
        let ab: Uint8Array = new Uint8Array(size);
        for (let i = 0; i < size; i++) {
            ab[i] = this.readByte();
        }
        return ab;
    }

    /** Reads a Byte */
    public readByte(): number {
        return this.dv.getUint8(this.curPos++);
    }

    /** Reads an SByte */
    public readSByte(): number {
        return this.dv.getInt8(this.curPos++);
    }

    /** Reads a UShort
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readUShort(isLittleEndian = this.isLittle): number {
        const ushort = this.dv.getUint16(this.curPos, isLittleEndian);
        this.curPos += 2;
        return ushort;
    }

    /** Reads a Short
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readShort(isLittleEndian = this.isLittle): number {
        const short = this.dv.getInt16(this.curPos, isLittleEndian);
        this.curPos += 2;
        return short;
    }

    /** Reads a UInt
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readUInt(isLittleEndian = this.isLittle): number {
        const uint = this.dv.getUint32(this.curPos, isLittleEndian);
        this.curPos += 4;
        return uint;
    }

    /** Reads an Int
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readInt(isLittleEndian = this.isLittle): number {
        const int = this.dv.getInt32(this.curPos, isLittleEndian);
        this.curPos += 4;
        return int;
    }

    /** Reads a UInt24
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readUInt24(isLittleEndian = this.isLittle): number {
        let uint24 = 0;
        if (isLittleEndian) {
            uint24 = this.readByte() |
                    (this.readByte() << 8) |
                    (this.readByte() << 16);
        } else {
            uint24 = (this.readByte() << 16) |
                    (this.readByte() << 8) |
                    this.readByte();
        }
        return uint24;
    }

    /** Reads an Int24
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readInt24(isLittleEndian = this.isLittle): number {
        let int24 = 0;
        if (isLittleEndian) {
            int24 = this.readByte() |
                    (this.readByte() << 8) |
                    (this.readByte() << 16);
        } else {
            int24 = (this.readByte() << 16) |
                    (this.readByte() << 8) |
                    this.readByte();
        }
        if (int24 & 0x800000) {
            int24 |= 0xFF000000;
        }
        return int24;
    }

    /** Reads a ULong
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readULong(isLittleEndian = this.isLittle): bigint {
        const ulong = this.dv.getBigUint64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return ulong;
    }

    /** Reads a Long
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readLong(isLittleEndian = this.isLittle): bigint {
        const long = this.dv.getBigInt64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return long;
    }

    /** Reads a Float
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readFloat(isLittleEndian = this.isLittle): number {
        const float = this.dv.getFloat32(this.curPos, isLittleEndian);
        this.curPos += 4;
        return float;
    }

    /** Reads a Double
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readDouble(isLittleEndian = this.isLittle): number {
        const double = this.dv.getFloat64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return double;
    }

    /** Reads a UTF16 string
     * @param Length length of the string in bytes, including null bytes
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public readString16(length: number = this.readUShort(), isLittleEndian = this.isLittle): string {
        let str = "";
        for (var i = 0; i < (length / 2); i++) {
            str += String.fromCharCode(this.readUShort(isLittleEndian));
        }
        return str.replace(/\x00/g, '');
    }

    /** Reads a UTF8 string
     * @param length Length of the string in bytes
    */
    public readString8(length: number = this.readByte()): string {
        let str = "";
        for (var i = 0; i < length; i++) {
            str += String.fromCharCode(this.readByte());
        }
        return str;
    }

    /** Reads a null-terminated UTF8 string */
    public readNullTerminatedString8(): string {
        let str = "";
        while (true) {
            const byte = this.readByte();
            if (byte === 0x00) {
                break;
            }
            str += String.fromCharCode(byte);
        }
        return str;
    }

    /** Reads a Byte without incrementing the position in the stream
     * @param offset Offset to read from
    */
    public peekByte(offset: number = this.curPos): number {
        return this.dv.getUint8(offset);
    }

    /** Reads an SByte without incrementing the position in the stream
     * @param offset Offset to read from
    */
    public peekSByte(offset: number = this.curPos): number {
        return this.dv.getInt8(offset);
    }

    /** Reads a UShort without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekUShort(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dv.getUint16(offset, isLittleEndian);
    }

    /** Reads a Short without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekShort(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dv.getInt16(offset, isLittleEndian);
    }

    /** Reads a UInt without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekUInt(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dv.getUint32(offset, isLittleEndian);
    }

    /** Reads an Int without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekInt(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dv.getInt32(offset, isLittleEndian);
    }

    /** Reads a ULong without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekULong(offset: number = this.curPos, isLittleEndian = this.isLittle): bigint {
        return this.dv.getBigUint64(offset, isLittleEndian);
    }

    /** Reads a Long without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekLong(offset: number = this.curPos, isLittleEndian = this.isLittle): bigint {
        return this.dv.getBigInt64(offset, isLittleEndian);
    }

    /** Reads a Float without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekFloat(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dv.getFloat32(offset, isLittleEndian);
    }

    /** Reads a Double without incrementing the position in the stream
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekDouble(offset: number = this.curPos, isLittleEndian = this.isLittle): number {
        return this.dv.getFloat64(offset, isLittleEndian);
    }

    /** Reads a UTF16 string without incrementing the position in the stream
     * @param length length of the string in bytes, including null bytes
     * @param offset Offset to read from
     * @param isLittleEndian Whether or not to read as Little Endian
    */
    public peekString16(length: number = this.readByte(), offset: number = this.curPos, isLittleEndian = this.isLittle): string {
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
    public peekString8(length: number = this.readByte(), offset: number = this.curPos) {
        let str = "";
        for (var i = 0; i < length; i++) {
            str += (String.fromCharCode(this.peekByte(offset + i)));
        }
        return str;
    }
}