//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include "IO/Serializable.h"

#include <Filesystem/Filesystem.h>
#include <libLCE.h>
#include <vector>

namespace lce::arc {
    /**
     * LCE Archive (.ARC) File
     * \brief A standard archive that can store files,
     * commonly used for storing game assets.
     *
     * ## Header
     * - File Count (`uint32_t`)
     *
     * ## File Index
     * For each file,
     * - Name Size (`uint16_t`)
     * - Name (`Name Size` bytes)
     * - File Offset (`uint32_t`)
     *   - Offset to the file data
     * - File Size (`uint32_t`)
     *
     * ## File Data
     * For each file,
     * - File Data (`File Size` bytes)
     */
    class LIBLCE_API Archive final : public fs::Filesystem,
                                     public io::Serializable {
      public:
        /** Creates an archive file with the contents of a physical folder */
        explicit Archive(const Filesystem &fs);
        /** Creates an empty archive file */
        Archive();

        /** Creates an archive file from serialized data */
        explicit Archive(std::vector<uint8_t> data);

        /** Creates an archive file from serialized data */
        explicit Archive(uint8_t *data);

        /** Calculates the total size of the archive file in bytes
         *
         * @returns The archive file size in bytes
         */
        size_t getSize() const override;

        /** Serializes the archive file
         *
         * @returns The serialized archive file
         */
        uint8_t *serialize() const override;

        friend std::wostream &operator<<(std::wostream &wos, const Archive &a) {
            wos << L"Archive[" << L"fileCount=" << a.getRoot()->getFileCount()
                << L"]";
            return wos;
        }
    };
} // namespace lce::arc

#endif // ARCHIVE_H
