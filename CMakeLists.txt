cmake_minimum_required(VERSION 3.26)
project(LCE VERSION 1.2.1)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCOMPILER_NAME="${CMAKE_CXX_COMPILER_ID}")
add_definitions(-DPLATFORM_NAME="${CMAKE_SYSTEM_NAME}")
add_definitions(-DPLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}")

message(STATUS "${CMAKE_SYSTEM_PROCESSOR}")

include_directories(include external)

# because _DEBUG is not universal
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCMAKE_BUILD_DEBUG)
endif ()

include_directories(external/emsdk/upstream/emscripten/system/include)
link_directories(external/emsdk/upstream/emscripten/system/lib)

set(LODEPNG_FILES
        external/lodepng/lodepng.h
        external/lodepng/lodepng.cpp
        external/lodepng/lodepng_util.h
        external/lodepng/lodepng_util.cpp)

set(ZLIB_FILES
        external/zlib/zlib.h
        external/zlib/inflate.h
        external/zlib/inflate.c
        external/zlib/adler32.c
        external/zlib/crc32.c
        external/zlib/zutil.c
        external/zlib/inffast.c
        external/zlib/inftrees.c
)

set(HEADERS
        include/libLCE.h
        include/libLCEExports.h
        include/World/Region.h
        include/World/Chunk.h
        include/Save/SaveFile.h
        include/IO/BinaryIO.h
        include/Filesystem/Filesystem.h
        include/Filesystem/File.h
        include/Archive/Archive.h
        include/Color/Color.h
        include/Color/ColorFile.h
        include/Localization/LocalizationFile.h
        include/Localization/Language.h
        include/Soundbank/Soundbank.h
        include/Compression/Compression.h
        include/Save/Thumb.h
        include/World/SplitSave.h
        include/World/Block.h
        include/Filesystem/FSObject.h
        include/Filesystem/Directory.h
        include/Bindings/EmscriptenBindings.h
)

set(FILES
        src/libLCEExports.cpp
        src/Bindings/EmscriptenBindings.cpp
        src/World/Region.cpp
        src/World/Chunk.cpp
        src/Save/SaveFile.cpp
        src/IO/BinaryIO.cpp
        src/Filesystem/Filesystem.cpp
        src/Archive/Archive.cpp
        src/Color/Colors/Color.cpp
        src/Color/Colors/WorldColor.cpp
        src/Color/ColorFile.cpp
        src/Color/ColorFileOld.cpp
        src/Localization/LocalizationFile.cpp
        src/Localization/Language.cpp
        src/Soundbank/Soundbank.cpp
        src/Save/SaveFileOld.cpp
        src/Save/SaveFileCommons.cpp
        src/IO/BinaryString.cpp
        src/Compression/Compression.cpp
        src/Save/Thumb.cpp
        src/World/SplitSave.cpp
        src/World/Block.cpp
        src/World/Chunk/V12.cpp
        src/Filesystem/FSObject.cpp
        src/Filesystem/File.cpp
        src/Filesystem/Directory.cpp
)

option(ENABLE_OPTIMIZATIONS "Enable compiler optimizations" OFF)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(ENVIRONMENT_TYPE "Emscripten")
    # weird?
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sNO_DISABLE_EXCEPTION_CATCHING -s USE_ZLIB=1 -s WASM_BIGINT -s MODULARIZE=1 -sEXPORT_ES6=1 -s EXPORT_ALL=1 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','getValue','setValue'] -s EXPORTED_FUNCTIONS=['_malloc'] -s ALLOW_MEMORY_GROWTH=1")
    add_executable(LCE ${HEADERS} ${FILES} ${LODEPNG_FILES})
    #    target_link_options(LCE PRIVATE --emit-tsd libLCE.d.ts)
else ()
    if (ENABLE_OPTIMIZATIONS STREQUAL "ON")
        add_compile_options(
                "$<$<COMPILE_LANGUAGE:CXX>:-O3;-ffast-math;-march=native;-fpic;-ftree-vectorize>"
        )
    endif ()
    unset(ENABLE_OPTIMIZATIONS CACHE)

    set(ENVIRONMENT_TYPE "Native")
    set(LIB_TYPE SHARED)
    add_library(LCE ${LIB_TYPE} ${HEADERS} ${FILES} ${LODEPNG_FILES} ${ZLIB_FILES})

    target_include_directories(LCE PUBLIC "include" "src")
    target_compile_definitions(LCE PRIVATE LIBLCE_EXPORTS)

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests EXCLUDE_FROM_ALL)
endif ()

# me: "why don't we use c++20 for std::endian?"
# them: "c++20 is in my nightmares"

#set endianness define
if (CMAKE_CXX_BYTE_ORDER EQUAL BIG_ENDIAN)
    target_compile_definitions(LCE PRIVATE BR_BIG_ENDIAN)
endif ()

target_compile_definitions(LCE PRIVATE LIBLCE_VERSION="${PROJECT_VERSION}")
