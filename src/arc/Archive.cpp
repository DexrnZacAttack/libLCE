//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "Archive.h"
#include "../io/BinaryIO.h"

namespace lce::arc {
    Archive::Archive(uint32_t fileCount, const std::vector<ArchiveFile> &index): fileCount(fileCount), index(index) {
    }

    Archive::Archive() = default;

    Archive Archive::readFromData(uint8_t *data) {
        Archive arc;
        io::BinaryIO io(data);

        arc.fileCount = io.readB<uint32_t>();

        std::vector<ArchiveFile> index(arc.fileCount);

        io::BinaryIO& io2 = io;

        for (uint32_t i = 0; i < arc.fileCount; i++) {
            ArchiveFile af = ArchiveFile::readArchiveFileBIO(io2);
            io.seek(af.offset);
            af.data = new uint8_t[af.size];
            io.readInto(af.data, af.size);
            index[i] = af;
        }

        arc.index = index;
        return arc;
    }
}
