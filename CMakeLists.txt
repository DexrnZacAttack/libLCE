cmake_minimum_required(VERSION 3.30)
project(lce)

set(CMAKE_CXX_STANDARD 17)

add_executable(lce library.cpp
        src/save/SaveFile.cpp
        src/save/SaveFile.h
        src/io/BinaryIO.cpp
        src/io/BinaryIO.h
        src/save/IndexInnerFile.cpp
        src/save/IndexInnerFile.h
        src/arc/Archive.cpp
        src/arc/Archive.h
        src/arc/ArchiveFile.cpp
        src/arc/ArchiveFile.h
        src/save/SaveFileOld.cpp
        src/save/SaveFileOld.h
        src/save/SaveFileCommons.cpp
        src/save/SaveFileCommons.h
)

# me: "why don't we use c++20 for std::endian?"
# them: "c++20 is in my nightmares"

#set endianness define
if (CMAKE_CXX_BYTE_ORDER EQUAL BIG_ENDIAN)
    target_compile_definitions(lce PRIVATE BR_BIG_ENDIAN)
endif ()