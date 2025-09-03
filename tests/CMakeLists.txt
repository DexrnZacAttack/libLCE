cmake_minimum_required(VERSION 3.26)
project(libLCE.Tests)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "${CMAKE_SYSTEM_PROCESSOR}")

add_executable(libLCE.Tests main.cpp)

target_link_libraries(libLCE.Tests PRIVATE LCE)

# ENABLE/DISABLE TESTS HERE
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
)