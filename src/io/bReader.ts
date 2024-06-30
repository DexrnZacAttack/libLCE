// Dexrn: Scuffed reader.

export class bReader {
    dvRead: DataView;
    curPos: number;
    isLittle: boolean = false;

    constructor(dvRead: DataView, isLittle: boolean, curPos = 0) {
        this.dvRead = dvRead;
        this.isLittle = isLittle;
        this.curPos = curPos;
    }

    getBuffer(): ArrayBuffer {
        return this.dvRead.buffer;
    }

    byteLength(): number {
        return this.dvRead.buffer.byteLength;
    }

    slice(start: number, end: number | undefined = undefined): ArrayBuffer {
        return this.dvRead.buffer.slice(start, end);
    }

    readByte(): number {
        return this.dvRead.getUint8(this.curPos++);
    }

    readSByte(): number {
        return this.dvRead.getInt8(this.curPos++);
    }

    readUShort(isLittleEndian = this.isLittle): number {
        const ushort = this.dvRead.getUint16(this.curPos, isLittleEndian);
        this.curPos += 2;
        return ushort;
    }

    readShort(isLittleEndian = this.isLittle): number {
        const short = this.dvRead.getInt16(this.curPos, isLittleEndian);
        this.curPos += 2;
        return short;
    }

    readUInt(isLittleEndian = this.isLittle): number {
        const uint = this.dvRead.getUint32(this.curPos, isLittleEndian);
        console.log(this.curPos);
        this.curPos += 4;
        return uint;
    }

    readInt(isLittleEndian = this.isLittle): number {
        const int = this.dvRead.getInt32(this.curPos, isLittleEndian);
        this.curPos += 4;
        return int;
    }

    readULong(isLittleEndian = this.isLittle): bigint {
        const ulong = this.dvRead.getBigUint64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return ulong;
    }

    readLong(isLittleEndian = this.isLittle): bigint {
        const long = this.dvRead.getBigInt64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return long;
    }

    readFloat(isLittleEndian = this.isLittle): number {
        const float = this.dvRead.getFloat32(this.curPos, isLittleEndian);
        this.curPos += 4;
        return float;
    }

    readDouble(isLittleEndian = this.isLittle): number {
        const double = this.dvRead.getFloat64(this.curPos, isLittleEndian);
        this.curPos += 8;
        return double;
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

    peekByte(): number {
        return this.dvRead.getUint8(this.curPos);
    }

    peekSByte(): number {
        return this.dvRead.getInt8(this.curPos);
    }

    peekUShort(isLittleEndian = this.isLittle): number {
        return this.dvRead.getUint16(this.curPos, isLittleEndian);
    }

    peekShort(isLittleEndian = this.isLittle): number {
        return this.dvRead.getInt16(this.curPos, isLittleEndian);
    }

    peekUInt(isLittleEndian = this.isLittle): number {
        return this.dvRead.getUint32(this.curPos, isLittleEndian);
    }

    peekInt(isLittleEndian = this.isLittle): number {
        return this.dvRead.getInt32(this.curPos, isLittleEndian);
    }

    peekULong(isLittleEndian = this.isLittle): bigint {
        return this.dvRead.getBigUint64(this.curPos, isLittleEndian);
    }

    peekLong(isLittleEndian = this.isLittle): bigint {
        return this.dvRead.getBigInt64(this.curPos, isLittleEndian);
    }

    peekFloat(isLittleEndian = this.isLittle): number {
        return this.dvRead.getFloat32(this.curPos, isLittleEndian);
    }

    peekDouble(isLittleEndian = this.isLittle): number {
        return this.dvRead.getFloat64(this.curPos, isLittleEndian);
    }
}