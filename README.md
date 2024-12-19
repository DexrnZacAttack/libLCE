# Experimental Rust Rewrite

### I am very new to rust dev, if you see things that are wrong please make some issue and explain, if I understand it well enough I will fix it.

there *WILL* be shitty code, I don't like that very much but idk common rust practices and I'm basically learning from
scratch.

## Progress

- [ ] Cleanup
- [ ] Console compression types
    - [ ] LZX/XMemCompress
    - [ ] Vita RLE
    - [X] Split Save RLE
    - [X] Chunk RLE
- [x] Savegame
    - [X] Reading
    - [X] Writing
    - [ ] Compression (only zlib currently)
    - [ ] Decompression
- [X] World info/thumbnail
    - [X] Reading
    - [X] Writing
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
    - [X] LOC
        - [X] Reading
        - [X] Writing
    - [ ] PCK
        - [ ] Reading
        - [ ] Writing
    - [ ] COL
        - [ ] Reading
        - [ ] Writing
    - [ ] MSSCMP
        - [X] Reading
        - [ ] Writing

## Legal Notice

This repo is not affiliated with Mojang Studios nor Microsoft. Minecraft is a trademark of Mojang Studios and Microsoft.