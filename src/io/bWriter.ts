// Dexrn: Scuffed BinaryWriter clone.
export class bWriter {
    dvWrite: DataView;
    curPos: number;
    isLittle: boolean;

    /** Creates an endianness-aware DataView with built in position tracking
     * @param dvRead The DataView
     * @param isLittle Whether or not to read as Little Endian
     * @param curPos Position to start at
    */
    constructor(dvWrite: DataView, isLittle: boolean = false, curPos = 0) {
        this.dvWrite = dvWrite;
        this.isLittle = isLittle;
        this.curPos = curPos;
    }

    /** Returns the ArrayBuffer of the entire stream. */
    getBuffer(): ArrayBuffer {
        return this.dvWrite.buffer;
    }

    /** Equivalent to ArrayBuffer.byteLength */
    byteLength(): number {
        return this.dvWrite.buffer.byteLength;
    }

    /** Equivalent to ArrayBuffer.slice
     * @param start Where to begin
     * @param end Where to end
    */
    slice(start: number, end: number | undefined = undefined): ArrayBuffer {
        return this.dvWrite.buffer.slice(start, end);
    }

    /** Writes a Byte
     * @param value The value to write.
    */
    writeByte(value: number): void {
        this.dvWrite.setUint8(this.curPos++, value);
    }

    /** Writes an SByte
     * @param value The value to write.
    */
    writeSByte(value: number): void {
        this.dvWrite.setInt8(this.curPos++, value);
    }

    /** Writes a UShort
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeUShort(value: number, isLittleEndian = this.isLittle): void {
        this.dvWrite.setUint16(this.curPos, value, isLittleEndian);
        this.curPos += 2;
    }

    /** Writes a Short
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeShort(value: number, isLittleEndian = this.isLittle): void {
        this.dvWrite.setInt16(this.curPos, value, isLittleEndian);
        this.curPos += 2;
    }

    /** Writes a UInt
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeUInt(value: number, isLittleEndian = this.isLittle): void {
        this.dvWrite.setUint32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
    }

    /** Writes an Int
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeInt(value: number, isLittleEndian = this.isLittle): void {
        const int = this.dvWrite.setInt32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
        int;
    }

    /** Writes a ULong
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeULong(value: bigint, isLittleEndian = this.isLittle): void {
        this.dvWrite.setBigUint64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
    }

    /** Writes a Long
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeLong(value: bigint, isLittleEndian = this.isLittle): void {
        this.dvWrite.setBigInt64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
    }

    /** Writes a Float
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeFloat(value: number, isLittleEndian = this.isLittle): void {
        this.dvWrite.setFloat32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
    }

    /** Writes a Double
     * @param value The value to write.
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeDouble(value: number, isLittleEndian = this.isLittle): void {
        this.dvWrite.setFloat64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
    }

    /** Writes a UTF16 string
     * @param string The string to write
     * @param isLittleEndian Whether or not to write as Little Endian
    */
    writeString16(string: string, isLittleEndian = this.isLittle) {
        ([...string]).forEach((char) => {
                this.writeShort((char.charCodeAt(0)), isLittleEndian);
        });
    }

    /** Writes a UTF8 string
     * @param string The string to write
    */
    writeString8(string: string) {
        ([...string]).forEach((char) => {
                this.writeByte(char.charCodeAt(0));
        });
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
}