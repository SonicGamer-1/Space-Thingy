cmake_minimum_required(VERSION 3.22.1)
project("space_game" C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")
set(GAME_SRC_DIR "${GAME_ROOT}/src")
set(GAME_ASSETS_DIR "${GAME_ROOT}/assets")
set(VENDOR_DIR "${GAME_ROOT}/android/vendor")

# SDL2 Options
set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL static library" FORCE)
set(SDL_TEST OFF CACHE BOOL "Build SDL tests" FORCE)
set(SDL2_DISABLE_INSTALL ON CACHE BOOL "Disable SDL2 install" FORCE)

# SDL2_image Options
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "Disable SDL2_image install" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "Disable SDL2_image samples" FORCE)
set(SDL2IMAGE_TESTS OFF CACHE BOOL "Disable SDL2_image tests" FORCE)
set(SDL2IMAGE_BACKEND_STB ON CACHE BOOL "Enable stb_image backend" FORCE)
set(SDL2IMAGE_PNG ON CACHE BOOL "Enable PNG support" FORCE)
set(SDL2IMAGE_JPG OFF CACHE BOOL "Disable JPG" FORCE)
set(SDL2IMAGE_AVIF OFF CACHE BOOL "Disable AVIF" FORCE)
set(SDL2IMAGE_WEBP OFF CACHE BOOL "Disable WEBP" FORCE)
set(SDL2IMAGE_TIF OFF CACHE BOOL "Disable TIFF" FORCE)
set(SDL2IMAGE_JXL OFF CACHE BOOL "Disable JXL" FORCE)
set(SDL2IMAGE_VENDORED OFF CACHE BOOL "Disable vendored image dependencies" FORCE)

# SDL2_mixer Options
set(SDL2MIXER_INSTALL OFF CACHE BOOL "Disable SDL2_mixer install" FORCE)
set(SDL2MIXER_SAMPLES OFF CACHE BOOL "Disable SDL2_mixer samples" FORCE)
set(SDL2MIXER_CMD OFF CACHE BOOL "Disable external player command" FORCE)
set(SDL2MIXER_FLAC OFF CACHE BOOL "Disable FLAC" FORCE)
set(SDL2MIXER_MOD OFF CACHE BOOL "Disable MOD" FORCE)
set(SDL2MIXER_MP3 OFF CACHE BOOL "Disable MP3" FORCE)
set(SDL2MIXER_MIDI OFF CACHE BOOL "Disable MIDI" FORCE)
set(SDL2MIXER_OPUS OFF CACHE BOOL "Disable OPUS" FORCE)
set(SDL2MIXER_WAVPACK OFF CACHE BOOL "Disable WAVPACK" FORCE)
set(SDL2MIXER_VORBIS "STB" CACHE STRING "Enable OGG Vorbis via stb_vorbis" FORCE)
set(SDL2MIXER_VENDORED OFF CACHE BOOL "Disable vendored mixer dependencies" FORCE)

# SDL2_ttf Options
set(SDL2TTF_INSTALL OFF CACHE BOOL "Disable SDL2_ttf install" FORCE)
set(SDL2TTF_SAMPLES OFF CACHE BOOL "Disable SDL2_ttf samples" FORCE)
set(SDL2TTF_HARFBUZZ OFF CACHE BOOL "Disable HarfBuzz" FORCE)
set(SDL2TTF_VENDORED ON CACHE BOOL "Vendored FreeType" FORCE)

# Add vendor subdirectories in order
add_subdirectory("${VENDOR_DIR}/SDL" "${CMAKE_CURRENT_BINARY_DIR}/SDL_build" EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/SDL_image" "${CMAKE_CURRENT_BINARY_DIR}/SDL_image_build" EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/SDL_mixer" "${CMAKE_CURRENT_BINARY_DIR}/SDL_mixer_build" EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/SDL_ttf" "${CMAKE_CURRENT_BINARY_DIR}/SDL_ttf_build" EXCLUDE_FROM_ALL)

# Setup compatibility include directory so #include <SDL2/SDL.h>, etc. resolve seamlessly
set(SDL2_COMPAT_DIR "${CMAKE_CURRENT_BINARY_DIR}/sdl2_compat")
file(MAKE_DIRECTORY "${SDL2_COMPAT_DIR}/SDL2")

file(WRITE "${SDL2_COMPAT_DIR}/SDL2/SDL.h" "#pragma once\n#include <SDL.h>\n")
file(WRITE "${SDL2_COMPAT_DIR}/SDL2/SDL_image.h" "#pragma once\n#include <SDL_image.h>\n")
file(WRITE "${SDL2_COMPAT_DIR}/SDL2/SDL_mixer.h" "#pragma once\n#include <SDL_mixer.h>\n")
file(WRITE "${SDL2_COMPAT_DIR}/SDL2/SDL_ttf.h" "#pragma once\n#include <SDL_ttf.h>\n")

# Main game shared library
add_library(main SHARED
    "${GAME_SRC_DIR}/Space.cpp"
    "${GAME_SRC_DIR}/Player.cpp"
    "${GAME_SRC_DIR}/Bullet.cpp"
    "${GAME_SRC_DIR}/SDLUtils.cpp"
    "${GAME_SRC_DIR}/UI.cpp"
)

target_include_directories(main PRIVATE
    "${SDL2_COMPAT_DIR}"
    "${GAME_SRC_DIR}"
    "${GAME_ASSETS_DIR}"
    "${VENDOR_DIR}/SDL_ttf"
)

target_link_libraries(main PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    android
    log
    GLESv1_CM
    GLESv2
    OpenSLES
)