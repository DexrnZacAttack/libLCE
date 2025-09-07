//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <Filesystem/Filesystem.h>
#include <libLCE.h>
#include <vector>

namespace lce::arc {

    class LIBLCE_API Archive final : public fs::Filesystem {
      public:
        /** Creates an archive file with the contents of a physical folder */
        explicit Archive(const Filesystem &fs);
        /** Creates an empty archive file */
        Archive();

        /** Creates an archive file from serialized data */
        explicit Archive(uint8_t *data);

        /** Calculates the total size of the archive file in bytes
         *
         * @returns The archive file size in bytes
         */
        [[nodiscard]] uint64_t getSize() const;

        /** Serializes the archive file
         *
         * @returns The serialized archive file
         */
        [[nodiscard]] uint8_t *serialize() const;

        friend std::wostream &operator<<(std::wostream &wos, const Archive &a) {
            wos << L"Archive[" << L"fileCount=" << a.getRoot()->getFileCount()
                << L"]";
            return wos;
        }
    };
} // namespace lce::arc

#endif // ARCHIVE_H
