# LibLCE
'Another port?', you may ask... Yup.

Downloads for Linux, Windows, and Emscripten are available at https://nightly.link/DexrnZacAttack/libLCE/workflows/cmake-multi-platform/main?preview

NOTE: Some of this project's work will go towards [Project Lodestone](https://github.com/Team-Lodestone) and [libLodestone](https://github.com/Team-Lodestone/libLodestone).   
Project Lodestone is an in-progress universal world converter, [check them out](https://team-lodestone.github.io)!

## Progress

- [x] Savegame
  - [X] Reading
  - [X] Writing
  - [ ] Compression
    - [ ] Zlib
    - [ ] LZX
    - [ ] Deflate (non-Zlib)
    - [ ] Split Save
    - [ ] Vita
    - [ ] Chunk
  - [X] Decompression
    - [X] Zlib
    - [ ] LZX
    - [ ] Deflate (non-Zlib)
    - [ ] Split Save
    - [X] Vita
    - [X] Chunk
- [ ] World info (THUMB)
  - [X] Reading
  - [ ] Writing
- [ ] World folder
  - [ ] Reading
  - [ ] Writing
- [ ] Region
  - [ ] Reading
    - [ ] Split Save
    - [X] Normal regions
  - [ ] Writing
- [ ] Chunks
  - [ ] Reading
    - [ ] V12
    - [ ] V11
    - [ ] V10
  - [ ] Writing
    - [ ] V12
    - [ ] V11
    - [ ] V10
- [ ] Other file formats
  - [X] ARC
    - [X] Reading
    - [X] Writing
  - [X] LOC
    - [X] Reading
    - [X] Writing
  - [ ] PCK
    - [ ] Reading
    - [ ] Writing
  - [X] COL
    - [X] Reading
    - [X] Writing
  - [ ] MSSCMP
    - [X] Reading
    - [ ] Writing
