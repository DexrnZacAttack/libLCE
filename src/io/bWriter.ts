// Dexrn: Scuffed Writeer.

export class bWriter {
    dvWrite: DataView;
    curPos: number;
    isLittle: boolean = false;

    constructor(dvWrite: DataView, isLittle: boolean, curPos = 0) {
        this.dvWrite = dvWrite;
        this.isLittle = isLittle;
        this.curPos = curPos;
    }

    getBuffer(): ArrayBuffer {
        return this.dvWrite.buffer;
    }

    byteLength(): number {
        return this.dvWrite.buffer.byteLength;
    }

    slice(start: number, end: number | undefined = undefined): ArrayBuffer {
        return this.dvWrite.buffer.slice(start, end);
    }

    writeByte(value: number): void {
        this.dvWrite.setUint8(this.curPos++, value);
    }

    writeSByte(value: number): void {
        this.dvWrite.setInt8(this.curPos++, value);
    }

    writeUShort(value: number, isLittleEndian = this.isLittle): void {
        const ushort = this.dvWrite.setUint16(this.curPos, value, isLittleEndian);
        this.curPos += 2;
        ushort;
    }

    writeShort(value: number, isLittleEndian = this.isLittle): void {
        const short = this.dvWrite.setInt16(this.curPos, value, isLittleEndian);
        this.curPos += 2;
        short;
    }

    writeUInt(value: number, isLittleEndian = this.isLittle): void {
        const uint = this.dvWrite.setUint32(this.curPos, value, isLittleEndian);
        console.log(this.curPos);
        this.curPos += 4;
        uint;
    }

    writeInt(value: number, isLittleEndian = this.isLittle): void {
        const int = this.dvWrite.setInt32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
        int;
    }

    writeULong(value: bigint, isLittleEndian = this.isLittle): void {
        const ulong = this.dvWrite.setBigUint64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
        ulong;
    }

    writeLong(value: bigint, isLittleEndian = this.isLittle): void {
        const long = this.dvWrite.setBigInt64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
        long;
    }

    writeFloat(value: number, isLittleEndian = this.isLittle): void {
        const float = this.dvWrite.setFloat32(this.curPos, value, isLittleEndian);
        this.curPos += 4;
        float;
    }

    writeDouble(value: number, isLittleEndian = this.isLittle): void {
        const double = this.dvWrite.setFloat64(this.curPos, value, isLittleEndian);
        this.curPos += 8;
        double;
    }

    incrementPos(num: number): void {
        this.curPos += num;
    }

    decrementPos(num: number): void {
        this.curPos -= num;
    }

    getPos(): number {
        return this.curPos;
    }
}