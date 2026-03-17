cmake_minimum_required(VERSION 3.26)
project(LCE VERSION 2.0.0)

set(VERSION_SUFFIX "-dev.6-lzx.1") # used for non-stable versions
set(CMAKE_CXX_STANDARD 20)

message(STATUS "${CMAKE_SYSTEM_PROCESSOR}")

include(FetchContent)
FetchContent_Declare(
        bio
        GIT_REPOSITORY https://codeberg.org/Dexrn/libBIO.git
        GIT_TAG 4.0.0-dev.3
)
FetchContent_MakeAvailable(bio)

include_directories(include external)

# because _DEBUG is not universal
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCMAKE_BUILD_DEBUG)
endif ()

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
        # Windows
        include/LCE/resources/resource.h
        include/LCE/resources/resource.rc

        # libLCE
        include/LCE/libLCE.h
        include/LCE/libLCEExports.h
        # Save
        include/LCE/save/SaveFile.h
        include/LCE/save/Thumb.h
        # IO
        include/LCE/io/Serializable.h
        # ARC
        include/LCE/archive/Archive.h
        # COL
        include/LCE/color/Color.h
        include/LCE/color/ColorFile.h
        # LOC
        include/LCE/localization/LocalizationFile.h
        include/LCE/localization/Language.h
        # MSSCMP
        include/LCE/soundbank/Soundbank.h
        include/LCE/soundbank/BinkaFile.h
        # Other
        include/LCE/compression/Compression.h
        include/LCE/compression/LzxCompression.h
        include/LCE/util/StringUtilities.h
        # Filesystem
        include/LCE/filesystem/FSObject.h
        include/LCE/filesystem/Directory.h
        include/LCE/filesystem/Filesystem.h
        include/LCE/filesystem/File.h
        # Unsorted
)

set(FILES
        src/libLCEExports.cpp
        src/save/SaveFile.cpp
        src/filesystem/Filesystem.cpp
        src/archive/Archive.cpp
        src/color/Colors/Color.cpp
        src/color/Colors/WorldColor.cpp
        src/color/ColorFile.cpp
        src/color/ColorFileOld.cpp
        src/localization/LocalizationFile.cpp
        src/localization/Language.cpp
        src/soundbank/Soundbank.cpp
        src/compression/Compression.cpp
        src/save/Thumb.cpp
        src/filesystem/FSObject.cpp
        src/filesystem/File.cpp
        src/filesystem/Directory.cpp
        src/io/Serializable.cpp
        src/util/StringUtilities.cpp
        src/compression/LzxCompression.cpp
)

option(ENABLE_OPTIMIZATIONS "Enable compiler optimizations" OFF)

if (ENABLE_OPTIMIZATIONS STREQUAL "ON")
    add_compile_options(
            "$<$<COMPILE_LANGUAGE:CXX>:-O3;-ffast-math;-march=native;-fpic;-ftree-vectorize>"
    )
endif ()
unset(ENABLE_OPTIMIZATIONS CACHE)

#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_compile_options(
#            "-D_GLIBCXX_DEBUG"
#    )
#endif ()

set(LIB_TYPE SHARED)
add_library(LCE ${LIB_TYPE} ${HEADERS} ${FILES} ${LODEPNG_FILES} ${ZLIB_FILES})

target_include_directories(LCE
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/LCE
)
target_compile_definitions(LCE PRIVATE LIBLCE_EXPORTS)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests EXCLUDE_FROM_ALL)
# me: "why don't we use c++20 for std::endian?"
# them: "c++20 is in my nightmares"

target_compile_features(bio PRIVATE cxx_std_20)
target_link_libraries(LCE PUBLIC bio-cpp20)

target_compile_definitions(LCE PUBLIC
        LIBLCE_COMPILER_NAME="${CMAKE_CXX_COMPILER_ID}"
        LIBLCE_PLATFORM_NAME="${CMAKE_SYSTEM_NAME}"
        LIBLCE_PLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}"
        LIBLCE_VERSION="${PROJECT_VERSION}${VERSION_SUFFIX}"
        LIBLCE_RC_VERSION=${PROJECT_VERSION_MAJOR},${PROJECT_VERSION_MINOR},${PROJECT_VERSION_PATCH}
)

# https://github.com/jothepro/doxygen-awesome-css?tab=readme-ov-file#cmake-with-fetchcontent
FetchContent_Declare(
        doxygen-awesome-css
        URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(doxygen-awesome-css)

FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

FIND_PACKAGE(Doxygen)
IF (DOXYGEN_FOUND)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    CONFIGURE_FILE(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    ADD_CUSTOM_TARGET(lceDocs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM
    )
ENDIF ()