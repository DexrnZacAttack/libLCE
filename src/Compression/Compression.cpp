//
// Created by DexrnZacAttack on 12/28/2024.
//

#include "Compression.h"

#include <iostream>
#include <fstream>

#include "../../external/zlib/zlib.h"
#include "../IO/BinaryIO.h"

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
    bool Compression::decompressZlibWithLength(std::vector<uint8_t>& in, std::vector<uint8_t>& out, uint32_t bufSize, uint32_t offset = 0) {
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

    bool Compression::decompressChunk(std::vector<uint8_t>& in, std::vector<uint8_t>& out) {
        io::BinaryIO io(in.data());

        int i = 0;

        // to sum it up: if read byte is not 255, push to out vec, otherwise read count and the value (if the count is 3+) and push it
        while (i < in.size()) {
            uint8_t byte = io.readByte();
            // if the byte isn't 0xFF (255)
            if (byte != 0xFF) {
                // push the byte that we just read to the output vector
                out[i++] = byte;
            } else {
                // otherwise read the count
                uint8_t val = 0;
                uint8_t count = io.readByte();

                // if the count is 3+ then we will read the value
                if (count >= 3) {
                    val = io.readByte();
                }

                // push to output vector count amount of times
                // literally spent forever debugging this, apparently the count needs to start at -1... ????
                for (int c = -1; c < count; c++)
                    out[i++] = val;
            }
        }

        return false;
    }
    /** Decompresses "Vita RLE"
     *
     * reversed from Compression::VitaVirtualDecompress from Wii U Edition
     *
     * The RLE works like so:
     * First, read a byte... if that byte is NOT 0, push to the output vector.
     *
     * Otherwise, read the next byte and do some weird ass bit shifting.
     *
     * The result of the bit shifting will be how many blocks of 8 zeros and normal zeros to add.
     *
     * Push those zeros to the output vector, and repeat.
     */
    bool Compression::decompressVita(std::vector<uint8_t> &in, std::vector<uint8_t> &out, uint32_t outBuf, uint32_t offset) {
        io::BinaryIO io(in.data());
        io.seek(offset);

        out.reserve(outBuf);

        while (io.getPosition() < in.size()) {
            // read a byte, if it's 0x00, then it needs to be decoded... otherwise copy it directly to the output vec.
            if (uint8_t byte = io.readByte(); byte == 0x00) {
                // determines how many zeros should be added, for some reason it looks like some weird ass math is involved...
                const uint8_t count = io.readByte();

                // how many blocks of 8 zeros to add
                uint8_t blks = count >> 3;
                // how many zeros to add
                uint8_t zeros = count & 7;

                // write blocks
                for (int i = 0; i < blks; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        out.push_back(0);
                    }
                }

                // write zeros
                for (int i = 0; i < zeros; ++i) {
                    out.push_back(0);
                }
            } else {
                // write the byte directly (no decoding needed)
                out.push_back(byte);
            }
        }

        return true;
    }

    bool Compression::decompress(std::vector<uint8_t> &in, std::vector<uint8_t> &out,
                                 compression::CompressionType type) {
        switch (type) {
            case compression::CompressionType::ZLIB: return decompressZlib(in, out); break;
            case compression::CompressionType::VITA: return decompressVita(in, out, 0); break;
        }
    }

    uint32_t Compression::getSizeFromSave(std::vector<uint8_t> &in, ByteOrder endian) {
        io::BinaryIO io(in.data());
        io.seek(4);

        return io.read<uint32_t>(endian);
    }
} // lce::compression