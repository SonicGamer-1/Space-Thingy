#pragma once

#include <SDL2/SDL_mixer.h>

// Audio
inline constexpr int idleVol = MIX_MAX_VOLUME / 3;
inline constexpr int moveVol = MIX_MAX_VOLUME / 2;
inline constexpr int bgmVol = MIX_MAX_VOLUME / 2;

// Window
inline constexpr int WIN_W = 1600;
inline constexpr int WIN_H = 900;
inline constexpr int BORDER_WIDTH = 5;

// Player
inline constexpr float SPEED = 800.0f;
inline constexpr float PLAYER_SIZE = 64.0f;
inline constexpr int PLAYER_MAX_HEALTH = 50;

// Enemies
inline constexpr float ENEMY_SIZE = 48.0f;
inline constexpr int ENEMY_NUMBER = 10;
inline constexpr float ENEMY_ACCELERATION = 500.0f;
inline constexpr float ENEMY_FRICTION = 0.91f;

// Animation
inline constexpr int SPRITE_FRAME_SIZE = 32;
inline constexpr float ANIMATION_FRAME_DURATION = 0.05f;

// Background
inline constexpr int STAR_MAX = 250;
inline constexpr float STAR_SPEED = 100.0f;

// Combat
inline constexpr int PLAYER_MAX_BULLETS = 10;
inline constexpr int PLAYER_BOUNCES = 2;
inline constexpr int PLAYER_SHOOT_COUNT = 3;
inline constexpr int PLAYER_BULLET_DAMAGE = 1;
inline constexpr int ENEMY_BOUNCES = 1;
inline constexpr int ENEMY_SHOOT_COUNT = 1;
inline constexpr int ENEMY_BULLET_DAMAGE = 1;

// Bullet effects
inline constexpr int BULLET_SIZE = 6;
inline constexpr int BULLET_DELAY = 150;
inline constexpr float BULLET_SPEED = 1000.0f;
inline constexpr int AURA = 3;
inline constexpr float TRAIL_INTERVAL = 0.01;
inline constexpr int TRAIL_SIZE = 25;
inline constexpr float TRAIL_ALPHA_POWER = 2.5f;
inline constexpr float TRAIL_MIN_SCALE = 0.2f;
inline constexpr float TRAIL_SCALE_RANGE = 0.8f;

// Engine effects
inline constexpr float ENGINE_VOLUME_ADJUSTMENT_SPEED = 7.0f;

// UI
inline constexpr int DEFAULT_FONT_SIZE = 28;
inline constexpr int HEALTH_PER_ICON = 5;
inline constexpr int HEALTH_ICON_SOURCE_SIZE = 16;
inline constexpr int GAME_RESET_PAUSE_MS = 300;
