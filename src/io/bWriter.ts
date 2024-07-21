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

// Dexrn: Scuffed BinaryWriter clone.
export class bWriter extends bCommons {
    // TODO: use sizable arrays instead of allocated at call time

    /** Creates an endianness-aware DataView with built in position tracking
     * @param dvWrite The DataView
     * @param isLittle Whether or not to read as Little Endian
     * @param curPos Position to start at
    */
    constructor(dvWrite: DataView, isLittle: boolean = false, curPos = 0) {
        super(dvWrite, isLittle, curPos);
    }

    /** Writes bytes from an array.
    * @param bytes Array of bytes to write
    */
    public write(bytes: Uint8Array): void {
        bytes.forEach(byte => {
            this.writeByte(byte);
        });
    }

    /** Writes bytes from an array.
    * @param byte Byte to write
    * @param amount Amount of bytes to write
    */
    public writeAmountOfBytes(byte: number, amount: number): void {
        for (var i = 0; i < amount; i++)
            this.writeByte(byte);
    }

    /** Writes a Byte
     * @param value The value to write.
    */
    public writeByte(value: number): void {
        this.dv.setUint8(this.curPos++, value);
    }

    /** Writes an SByte
     * @param value The value to write.
    */
    public writeSByte(value: number): void {
        this.dv.setInt8(this.curPos++, value);
    }

    /** Writes a UShort
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeUShort(value: number, isLittleEndian = this.isLittle): void {
        this.dv.setUint16(this.curPos, value, isLittleEndian);
        this.curPos += 2;
    }

    /** Writes a Short
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeShort(value: number, isLittleEndian = this.isLittle): void {
        this.dv.setInt16(this.curPos, value, isLittleEndian);
        this.curPos += 2;
    }

    /** Writes a UInt
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeUInt(value: number, isLittleEndian = this.isLittle): void {
        this.dv.setUint32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
    }

    /** Writes an Int
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeInt(value: number, isLittleEndian = this.isLittle): void {
        const int = this.dv.setInt32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
        return int;
    }

    /** Writes a ULong
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeULong(value: bigint, isLittleEndian = this.isLittle): void {
        this.dv.setBigUint64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
    }

    /** Writes a Long
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeLong(value: bigint, isLittleEndian = this.isLittle): void {
        this.dv.setBigInt64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
    }

    /** Writes a Float
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeFloat(value: number, isLittleEndian = this.isLittle): void {
        this.dv.setFloat32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
    }

    /** Writes a Double
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeDouble(value: number, isLittleEndian = this.isLittle): void {
        this.dv.setFloat64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
    }

    /** Writes a UTF16 string
     * @param string The string to write
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    public writeString16(string: string, isLittleEndian = this.isLittle) {
        ([...string]).forEach((char) => {
                this.writeShort((char.charCodeAt(0)), isLittleEndian);
        });
    }

    /** Writes a UTF8 string
     * @param string The string to write
     * @param isNullTerminated Determines whether or not to append a null byte to the end of the string
    */
    public writeString8(string: string, isNullTerminated: boolean = false) {
        ([...string]).forEach((char) => {
                this.writeByte(char.charCodeAt(0));
        });
        if (isNullTerminated) {
            this.writeByte(0);
        }
    }

}