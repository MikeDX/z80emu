# ZXEMU

Version 0.0.2  
https://github.com/MikeDX/z80emu/  
Copyright (c) 2016 MikeDX  

Uses Z80 Cpu Core by Adrian Brown 

ZXEMU is a free open source cross platform ZX Spectrum emulator, currently work in progress hoping to support various models of ZX Spectrum.

[Play online](http://js.mikedx.co.uk/zxem.html)

[![build](https://github.com/MikeDX/z80emu/actions/workflows/build.yml/badge.svg)](https://github.com/MikeDX/z80emu/actions/workflows/build.yml)

## Building

Install SDL 1.2 (or SDL2) first:

- Ubuntu/Debian: `sudo apt-get install libsdl1.2-dev libsdl2-dev`
- macOS (Homebrew): `brew install sdl12-compat sdl2`

Then:

```
make            # zxem with SDL 1.2
make SDL=2      # zxem with SDL2
make DEBUG=1    # debug build
make PLAT=HTML  # Emscripten build (zxem.html)
make test       # build and run the CPU test suite
```

Run `./zxem` from the repository root (it loads `roms/48k.rom`).

## TODO list
- [x] Working Z80 CPU Core  
- [x] Screen renderer (pixels, ink, paper, bright, flash)
- [x] Keyboard input  
- [x] Spectrum 48k rom support  
- [x] .scr image viewer
- [x] Windows port  
- [x] OSX port  
- [x] Linux  
- [x] Android port  
- [x] Html/Javascript port  (compile with make PLAT=HTML)
- [x] Raspberry Pi port  
- [ ] 'BEEP' sound output  
- [ ] Spectrum 128K support  
- [ ] AY-3-8912 emulation  
- [ ] Configurable gamepad  
- [ ] TZX Support  
- [ ] Z80 Snapshot support  
- [ ] SNA Snapshot support  
- [ ] TAP Support  

Please help to test and develop this project!
