#pragma once
#include <SDL2/SDL_mixer.h>

// Sounds
inline constexpr int idleVol = MIX_MAX_VOLUME / 3;
inline constexpr int moveVol = MIX_MAX_VOLUME / 2;
inline constexpr int bgmVol = MIX_MAX_VOLUME / 2;

// Player
inline constexpr float SPEED = 800.0f;
inline constexpr float PLAYER_SIZE = 64.0f;
inline constexpr float ENEMY_SIZE = 48.0f;
inline constexpr int ENEMY_NUMBER = 10;	

// Stars
inline constexpr int STAR_MAX = 250;

// Bullets
inline constexpr int MAX_BULLETS = 10;
inline constexpr int BULLET_SIZE = 6;
inline constexpr int BULLET_DELAY = 150;
inline constexpr float BULLET_SPEED = 1000.0f;

// Effects
inline constexpr int AURA = 3;
inline constexpr float TRAIL_INTERVAL = 0.01;
inline constexpr int TRAIL_SIZE = 25;

// Window
inline constexpr int WIN_H = 900;
inline constexpr int WIN_W = 1600;
inline constexpr int BORDER_WIDTH = 5;
