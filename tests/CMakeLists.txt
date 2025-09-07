cmake_minimum_required(VERSION 3.26)
project(libLCE.Tests)

set(CMAKE_CXX_STANDARD 17)

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

target_link_libraries(libLCE.Tests PRIVATE LCE)

target_include_directories(libLCE.Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Should we write the entire filesystem to the drive
target_compile_definitions(libLCE.Tests PRIVATE
        WRITE_FS=false)

# ENABLE/DISABLE TESTS HERE
target_compile_definitions(libLCE.Tests PRIVATE
        RUN_FORMATS_TESTS=true
        RUN_VFS_TESTS=true
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
        BE_TO_LE_SAVEGAME=true
        LE_TO_BE_SAVEGAME=true
        PRERELEASE_SAVEGAME=true
        PSVITA_SAVEGAME=true
        ARC=true
        LOC=true
        COL=true
        BE_THUMB=true
        LE_THUMB=true
        SWITCH_4B_WCHAR_THUMB=true
        READ_COMPRESSED_CHUNK=true
        READ_REGION=true
        NEWGEN_MSSCMP=true
        OLDGENMSSCMP=true
        SAVEGAME_FROM_FOLDER=true
)