# libLCE

A TS library for Minecraft Legacy Console Edition files + more 

> [!NOTE]
> This project is still in development, check out the [progress](#progress) below.

![Progress bar](/assets/cur_progress.png)

[![npm](https://img.shields.io/npm/v/liblce.svg)](https://www.npmjs.com/package/liblce)
[![downloads](https://img.shields.io/npm/dm/liblce.svg)](https://www.npmjs.com/package/liblce)

## Progress

- [ ] Console compression types
  - [ ] LZX/XMemCompress
  - [X] Vita RLE (needs rewrite)
  - [X] PS4/Switch RLE
  - [ ] Chunk RLE 
- [x] Savegame
  - [X] Reading
  - [X] Writing
  - [X] Compression
  - [ ] Decompression (only ZLib works by default, you can still call the DecompressVitaRle, etc functions for decompressing saves.) 
- [ ] World info
  - [ ] Reading
  - [ ] Writing
- [ ] World folder
  - [ ] Reading
  - [ ] Writing
- [ ] Region
  - [ ] Reading
  - [ ] Writing
- [ ] Chunks
  - [ ] Reading
  - [ ] Writing
- [ ] Other file formats
  - [X] ARC
    - [X] Reading
    - [X] Writing
  - [ ] LOC
    - [X] Reading
    - [ ] Writing
  - [ ] PCK
    - [ ] Reading
    - [ ] Writing
  - [ ] MSSCMP
    - [X] Reading
    - [ ] Writing
  
## Credits

- ### ![Offroaders123](https://avatars.githubusercontent.com/Offroaders123?size=32) [Offroaders123](https://github.com/offroaders123)
- ### [LegacyEditor](https://github.com/zugebot/LegacyEditor) - A lot of compression stuff was ported from this project
- ### ![zugebot](https://avatars.githubusercontent.com/zugebot?size=32) [zugebot](https://github.com/zugebot)
- ### ![UtterEvergreen1](https://avatars.githubusercontent.com/UtterEvergreen1?size=32) [UtterEvergreen1](https://github.com/UtterEvergreen1) - Documentation + other help
- ### ![MisledWater79](https://avatars.githubusercontent.com/MisledWater79?size=32) [MisledWater79](https://github.com/MisledWater79) - Help with region stuffs
- ### ![EternalModz](https://avatars.githubusercontent.com/EternalModz?size=32) [EternalModz](https://github.com/EternalModz)
- ### ![PhoenixARC](https://avatars.githubusercontent.com/PhoenixARC?size=32) [PhoenixARC](https://github.com/PhoenixARC) - Documentation + other help
- ### ![Project Lodestone](https://avatars.githubusercontent.com/Team-Lodestone?size=32) [Project Lodestone](https://github.com/Team-Lodestone)
- ### [OMI-Filetype-Library](https://github.com/PhoenixARC/OMI-Filetype-Library)

## Legal Notice

This repo is not affiliated with Mojang Studios nor Microsoft. Minecraft is a trademark of Mojang Studios and Microsoft.
