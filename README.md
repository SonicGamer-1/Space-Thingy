# Space Game

A 2D arcade space combat game written in C++17 using the **SDL2** library
suite.

## Features

- **Space Physics & Movement**: Smooth player movement with engine sound
  dynamics.
- **Combat Systems**: Projectile physics with trails, wall bouncing, enemy AI,
  and enemy counter-bullets.
- **Bullet States**: Player bullets have one remaining bounce and change from
  green to yellow; enemy bullets are red and do not bounce.
- **Audio System**: Background music and embedded sound effects managed with
  `SDL_mixer`.
- **UI & Font Rendering**: Health display, bullet inventory, and FPS counter
  using `SDL_ttf`.
- **Window and Rendering**: Resizable, DPI-aware SDL window with fixed logical
  1600x900 rendering and vsync disabled.
- **Embedded and External Assets**: External music alongside generated
  byte-array asset headers.

## Project Structure

```text
Space/
├── assets/
│   ├── BGM.ogg
│   ├── BulletSounds.h
│   ├── PlayerSounds.h
│   ├── Roboto-Regular.h
│   ├── SpaceShip.h
│   └── uiTex.h
├── bin/
│   ├── game.exe
│   └── game
├── build/
│   ├── windows/
│   └── linux/
├── src/
│   ├── Bullet.cpp / .h
│   ├── Constants.h
│   ├── Player.cpp / .h
│   ├── SDLUtils.cpp / .h
│   ├── Space.cpp
│   └── UI.cpp / .h
├── Makefile
└── README.md
```

## Prerequisites

### Windows

1. MinGW-w64 with C++17 support.
2. GNU Make (`mingw32-make`).
3. SDL2 development libraries:
   - SDL2
   - SDL2_image
   - SDL2_ttf
   - SDL2_mixer

### Linux

1. GCC with C++17 support.
2. GNU Make.
3. `pkg-config`.
4. SDL2 development packages:
   - `libsdl2-dev`
   - `libsdl2-image-dev`
   - `libsdl2-ttf-dev`
   - `libsdl2-mixer-dev`

## Building and Running

The Makefile supports explicit Windows and Linux builds. Object files are
kept separate under `build/windows` and `build/linux`.

### Windows

Build:

```bash
mingw32-make windows
```

Build and run:

```bash
mingw32-make PLATFORM=windows run
```

### Linux

Build:

```bash
make linux
```

Build and run:

```bash
make PLATFORM=linux run
```

### Host platform

Build for the detected host platform:

```bash
make
```

Clean generated build artifacts:

```bash
make clean
```

On Windows, use `mingw32-make clean`.

## Gameplay

- Move with **WASD**.
- Aim with the mouse.
- Click the left mouse button to fire.
- Player bullets can bounce once:
  - Green: one remaining bounce.
  - Yellow: zero remaining bounces.
- Enemy bullets are red and do not bounce.

## Rendering

- Fixed logical resolution: `1600x900`.
- Resizable window with logical game scaling.
- Windows per-monitor v2 DPI awareness.
- Renderer vsync explicitly disabled.
- Nearest-neighbor logical scaling.

## Music Attribution

- **BGM**: *Shadows And Dust* by Scott Buckley
  ([www.scottbuckley.com.au](https://www.scottbuckley.com.au))
- Promoted by [Chosic](https://www.chosic.com/free-music/all/)
- Creative Commons CC BY 4.0
  ([license](https://creativecommons.org/licenses/by/4.0/))
