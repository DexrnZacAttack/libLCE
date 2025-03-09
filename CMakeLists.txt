cmake_minimum_required(VERSION 3.30)
project(lce)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(ENVIRONMENT_TYPE "Emscripten")
    # weird?
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MODULARIZE=1 -sEXPORT_ES6=1 -s EXPORT_ALL=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap','getValue','setValue'] -s EXPORTED_FUNCTIONS=['_malloc'] -s ALLOW_MEMORY_GROWTH=1")
    set(LIB_TYPE STATIC)
else ()
    set(ENVIRONMENT_TYPE "Native")
    set(LIB_TYPE SHARED)
endif ()

add_library(lce ${LIB_TYPE}
        src/world/region.cpp
        src/world/region.h
        src/world/Chunk.cpp
        src/world/Chunk.h
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
        src/io/BinaryString.cpp
        src/compression/Compression.cpp
        src/compression/Compression.h
        src/compression/Chunk.cpp
        src/compression/Chunk.h
        src/lce.h
        src/col/Color.cpp
        src/col/Color.h
        src/col/ColorFileOld.cpp
        src/col/ColorFileOld.h
        src/col/ColorFileCommons.cpp
        src/col/ColorFileCommons.h
        src/col/ColorFile.cpp
        src/col/ColorFile.h
        src/col/ColorCommons.cpp
        src/col/ColorCommons.h
        src/col/WorldColor.cpp
        src/col/WorldColor.h
        src/io/ByteOrder.h
)

add_executable(tests
        src/world/region.cpp
        src/world/region.h
        src/world/Chunk.cpp
        src/world/Chunk.h
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
        src/io/BinaryString.cpp
        src/compression/Compression.cpp
        src/compression/Compression.h
        src/compression/Chunk.cpp
        src/compression/Chunk.h
        src/col/Color.cpp
        src/col/Color.h
        src/col/ColorFileOld.cpp
        src/col/ColorFileOld.h
        src/col/ColorFileCommons.cpp
        src/col/ColorFileCommons.h
        src/col/ColorFile.cpp
        src/col/ColorFile.h
        src/col/ColorCommons.cpp
        src/col/ColorCommons.h
        src/col/WorldColor.cpp
        src/col/WorldColor.h
        src/io/ByteOrder.h
tests.cpp)

target_link_libraries(tests PRIVATE lce)

# me: "why don't we use c++20 for std::endian?"
# them: "c++20 is in my nightmares"

#set endianness define
if (CMAKE_CXX_BYTE_ORDER EQUAL BIG_ENDIAN)
    target_compile_definitions(lce PRIVATE BR_BIG_ENDIAN)
endif ()