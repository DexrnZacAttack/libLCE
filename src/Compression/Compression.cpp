//
// Created by DexrnZacAttack on 12/28/2024.
//

#include <Compression/Compression.h>
#include <IO/BinaryIO.h>
#include <zlib/zlib.h>

#include <fstream>
#include <iostream>

namespace lce::compression {
    bool Compression::decompressZlib(std::vector<uint8_t>& in, std::vector<uint8_t>& out) {
        if (in.size() < 2) {
            std::cerr << "Input is too small." << std::endl;
            return false;
        }

        z_stream stream = {};
        int res = inflateInit(&stream);
        if (res != Z_OK) {
            std::cerr << "Failed to set up Zlib" << std::endl;
            return false;
        }

        stream.avail_in = in.size();
        stream.next_in = &in[0];

        size_t dBuff = 1024;
        out.resize(dBuff);

        stream.avail_out = dBuff;
        stream.next_out = out.data();

        while (true) {
            res = inflate(&stream, Z_NO_FLUSH);
            if (res == Z_DATA_ERROR || res == Z_MEM_ERROR) {
                std::cerr << "Zlib error: " << res << std::endl;
                inflateEnd(&stream);
                return false;
            }

            if (res == Z_STREAM_END) {
                break;
            }

            if (stream.avail_out == 0) {
                dBuff *= 2;
                out.resize(dBuff);
                stream.avail_out = dBuff - stream.total_out;
                stream.next_out = out.data() + stream.total_out;
            }
        }

        out.resize(stream.total_out);

        inflateEnd(&stream);
        return true;
    }

    /** Decompresses ("inflates") zlib-compressed data
     * @param in Input data
     * @param out Where to output to
     * @param bufSize The size of the output buffer
     * @param offset The offset we want to read from
     *
     * @returns false — If there is no error
     */
    bool Compression::decompressZlibWithLength(std::vector<uint8_t>& in, std::vector<uint8_t>& out, uint32_t bufSize,
                                               uint32_t offset = 0) {
        if (in.size() < 2) {
            std::cerr << "Input is too small" << std::endl;
            return true;
        }

        if (bufSize == 0) {
            std::cerr << "Cannot decompress into buffer with no size" << std::endl;
            return true;
        }

        z_stream stream = {};
        int res = inflateInit(&stream);
        if (res != Z_OK) {
            std::cerr << "Failed to set up Zlib" << std::endl;
            return true;
        }

        stream.avail_in = in.size() - offset;
        stream.next_in = &in[offset];

        out.resize(bufSize);

        stream.avail_out = bufSize;
        stream.next_out = out.data();

        res = inflate(&stream, Z_NO_FLUSH);
        if (res != Z_OK && res != Z_STREAM_END) {
            std::cerr << "Zlib failed to decompress: " << res << std::endl;
            inflateEnd(&stream);
            return true;
        }

        inflateEnd(&stream);
        return false;
    }

    // Modified version of fully decompiled & matching Compression::internalDecompressRle from Nintendo Switch Edition
    bool Compression::decompressChunk(std::vector<uint8_t>& in, std::vector<uint8_t>& out) {
        // todo: could make it auto allocate out to 0x40000 like LCE does
        io::BinaryIO io(in.data());

        int i = 0;

        // to sum it up: read byte, if byte is not 255, push to out vec, otherwise read count and the value (if the
        // count is 3+, otherwise value is 0xFF) and push it
        while (i < in.size()) {
            uint8_t byte = io.readByte();
            // if the byte isn't 0xFF (255)
            if (byte == 0xFF) {
                // otherwise read the count
                uint8_t count = io.readByte();
                uint8_t val;

                // if the count is 3+ then we will read the value
                // otherwise it is 0xFF
                if (count < 3) {
                    val = 0xFF;
                } else {
                    val = io.readByte();
                }

                // push to output vector count amount of times
                for (int c = 0; c <= count; ++c)
                    out[i++] = val;
            } else {
                // push the byte that we just read to the output vector
                out[i++] = byte;
            }
        }

        return false;
    }

    /** Decompresses "Vita RLE"
     *
     * Modified version of fully decompiled & matching Compression::VitaVirtualDecompress from Nintendo Switch Edition
     *
     * The RLE works like so:
     * First, read a byte... if that byte is NOT 0, push to the output vector.
     *
     * Otherwise, read the next byte, that's the count of how many zeros to push.
     *
     * Push those zeros to the output vector, and repeat.
     *
     * It's much simpler than the pseudocode made it look.
     */
    bool Compression::decompressVita(std::vector<uint8_t>& in, std::vector<uint8_t>& out, uint32_t outBuf,
                                     uint32_t offset) {
        // todo: could make it auto allocate out to 0x40000 like LCE does
        io::BinaryIO io(in.data());
        io.seek(offset);

        out.reserve(outBuf);

        while (io.getPosition() < in.size()) {
            // read a byte, if it's 0x00, then it needs to be decoded... otherwise copy it directly to the output vec.
            if (uint8_t byte = io.readByte(); byte == 0x00) {
                // determines how many zeros should be added
                const uint8_t count = io.readByte();

                // write zeros
                for (int z = 0; z < count; ++z) {
                    out.push_back(0);
                }
            } else {
                // write the byte directly (no decoding needed)
                out.push_back(byte);
            }
        }

        return true;
    }

    bool Compression::decompress(std::vector<uint8_t>& in, std::vector<uint8_t>& out,
                                 compression::CompressionType type) {
        switch (type) {
        case compression::CompressionType::ZLIB:
            return decompressZlib(in, out);
            break;
        case compression::CompressionType::VITA:
            return decompressVita(in, out, 0);
            break;
        default:
            throw std::runtime_error("Invalid compression type");
        }
    }

    uint32_t Compression::getSizeFromSave(std::vector<uint8_t>& in, io::ByteOrder endian) {
        io::BinaryIO io(in.data());
        io.seek(4);

        return io.read<uint32_t>(endian);
    }
} // namespace lce::compression
