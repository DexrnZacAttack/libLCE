cmake_minimum_required(VERSION 3.26)
project(libLCE.Tests)

set(CMAKE_CXX_STANDARD 20)

message(STATUS "${CMAKE_SYSTEM_PROCESSOR}")

set(FILES
        main.cpp
        util.h
        tests/formats.h
        tests/formats.cpp
        tests/vfs.cpp
        tests/vfs.h
)

add_executable(libLCE.Tests ${FILES})

target_link_libraries(libLCE.Tests PRIVATE LCE bio)

target_include_directories(libLCE.Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Should we allow for exceptions to throw for debugging
target_compile_definitions(libLCE.Tests PRIVATE
        ALLOW_THROW=true)

# Should we write the entire filesystem to the drive
target_compile_definitions(libLCE.Tests PRIVATE
        WRITE_FS=false
        WRITE_LOC=false)

# ENABLE/DISABLE TESTS HERE
target_compile_definitions(libLCE.Tests PRIVATE
        RUN_FORMATS_TESTS=true
        RUN_VFS_TESTS=false
)

# select vfs tests to run
target_compile_definitions(libLCE.Tests PRIVATE
        FOREACH_ALL=true
        FOREACH_FILES=true
        FOREACH_DIRS=true
        FOREACH_ALL_RECURSE=true
        FOREACH_FILES_RECURSE=true
        FOREACH_DIRS_RECURSE=true
        FILE_FROM_REAL_FILE=true
)

# select formats tests to run
target_compile_definitions(libLCE.Tests PRIVATE
        BE_SAVEGAME=true
        LE_SAVEGAME=true
        BE_TO_LE_SAVEGAME=false
        LE_TO_BE_SAVEGAME=false
        PRERELEASE_SAVEGAME=false
        PSVITA_SAVEGAME=false
        ARC=true
        LOC=false
        COL=false
        BE_THUMB=false
        LE_THUMB=false
        SWITCH_4B_WCHAR_THUMB=false
        READ_COMPRESSED_CHUNK=false
        READ_REGION=false
        NEWGEN_MSSCMP=false
        OLDGENMSSCMP=false
        SAVEGAME_FROM_FOLDER=false
)