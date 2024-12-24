cmake_minimum_required(VERSION 3.30)
project(lce)

set(CMAKE_CXX_STANDARD 17)

add_library(lce SHARED
        src/save/SaveFile.cpp
        src/save/SaveFile.h
        src/io/BinaryIO.cpp
        src/io/BinaryIO.h
        src/save/IndexInnerFile.cpp
        src/save/IndexInnerFile.h
        src/arc/Archive.cpp
        src/arc/Archive.h
        src/arc/ArchiveInnerFile.cpp
        src/arc/ArchiveInnerFile.h
        src/save/SaveFileOld.cpp
        src/save/SaveFileOld.h
        src/save/SaveFileCommons.cpp
        src/save/SaveFileCommons.h
)

add_executable(tests
tests.cpp)

target_link_libraries(tests PRIVATE lce)

# me: "why don't we use c++20 for std::endian?"
# them: "c++20 is in my nightmares"

#set endianness define
if (CMAKE_CXX_BYTE_ORDER EQUAL BIG_ENDIAN)
    target_compile_definitions(lce PRIVATE BR_BIG_ENDIAN)
endif ()