cmake_minimum_required(VERSION 3.26)
project(libLCE)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCOMPILER_NAME="${CMAKE_CXX_COMPILER_ID}")
add_definitions(-DPLATFORM_NAME="${CMAKE_SYSTEM_NAME}")
add_definitions(-DPLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}")

# because _DEBUG is not universal
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCMAKE_BUILD_DEBUG)
endif()

include_directories(external/emsdk/upstream/emscripten/system/include)
link_directories(external/emsdk/upstream/emscripten/system/lib)

#set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)
#set(ZLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/zlib)

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

set(FILES
        src/libLCE.h
        src/libLCEExports.h
        src/libLCEExports.cpp
        src/EmscriptenBindings.cpp
        src/World/Region.cpp
        src/World/Region.h
        src/World/Chunk.cpp
        src/World/Chunk.h
        src/Save/SaveFile.cpp
        src/Save/SaveFile.h
        src/IO/BinaryIO.cpp
        src/IO/BinaryIO.h
        src/Save/IndexInnerFile.cpp
        src/Save/IndexInnerFile.h
        src/File/File.cpp
        src/File/File.h
        src/File/InnerFile.cpp
        src/File/InnerFile.h
        src/Archive/Archive.cpp
        src/Archive/Archive.h
        src/Colour/Colours/Colour.cpp
        src/Colour/Colours/Colour.h
        src/Colour/ColourFileOld.cpp
        src/Colour/ColourFileOld.h
        src/Colour/ColourFileCommons.cpp
        src/Colour/ColourFileCommons.h
        src/Colour/ColourFile.cpp
        src/Colour/ColourFile.h
        src/Colour/Colours/ColourCommons.cpp
        src/Colour/Colours/ColourCommons.h
        src/Colour/Colours/WorldColour.cpp
        src/Colour/Colours/WorldColour.h
		src/Localization/LocalizationFile.cpp
        src/Localization/LocalizationFile.h
        src/Localization/Language.cpp
		src/Localization/Language.h
		src/Save/SaveFileOld.cpp
        src/Save/SaveFileOld.h
        src/Save/SaveFileCommons.cpp
        src/Save/SaveFileCommons.h
        src/IO/BinaryString.cpp
        src/Compression/Compression.cpp
        src/Compression/Compression.h
        src/IO/ByteOrder.h
        src/Save/Thumb.cpp
        src/Save/Thumb.h
        src/World/SplitSave.cpp
        src/World/SplitSave.h
        src/World/Block.cpp
        src/World/Block.h
        src/World/Chunk/V12.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(ENVIRONMENT_TYPE "Emscripten")
    # weird?
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sNO_DISABLE_EXCEPTION_CATCHING -s USE_ZLIB=1 -s WASM_BIGINT -s MODULARIZE=1 -sEXPORT_ES6=1 -s EXPORT_ALL=1 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','getValue','setValue'] -s EXPORTED_FUNCTIONS=['_malloc'] -s ALLOW_MEMORY_GROWTH=1")
    add_executable(libLCE ${FILES} ${LODEPNG_FILES})
#    target_link_options(libLCE PRIVATE --emit-tsd libLCE.d.ts)
else ()
    set(ENVIRONMENT_TYPE "Native")
    set(LIB_TYPE SHARED)
    add_library(libLCE ${LIB_TYPE} ${FILES} ${LODEPNG_FILES} ${ZLIB_FILES})

    target_compile_definitions(libLCE PRIVATE LIBLCE_EXPORTS)

    add_executable(tests tests.cpp)

    target_link_libraries(tests PRIVATE libLCE)

endif ()

# me: "why don't we use c++20 for std::endian?"
# them: "c++20 is in my nightmares"

#set endianness define
if (CMAKE_CXX_BYTE_ORDER EQUAL BIG_ENDIAN)
    target_compile_definitions(libLCE PRIVATE BR_BIG_ENDIAN)
endif ()
