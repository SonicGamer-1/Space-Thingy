#include "Bullet.h"
#include <algorithm>
#include <cmath>

Bullet::Bullet(float startX, float startY, float dirx, float diry, float s)
    : x(startX), y(startY), speed(s), alive(true), bounces(PLAYER_BOUNCES),
      trailTimer(0.0f) {
  vx = dirx * speed;
  vy = diry * speed;
  textureIndex = 0;
}
EnemyBullet::EnemyBullet(float startX, float startY, float dirx, float diry,
                         float s)
    : Bullet(startX, startY, dirx, diry, s) // call base constructor
{
  bounces = ENEMY_BOUNCES; // enemy bullets have fewer bounces
  textureIndex = 1;
}

void Bullet::update(float dt) {
  if (!alive)
    return;

  x += vx * dt;
  y += vy * dt;

  float r = BULLET_SIZE / 2.0f + AURA;

  if (x - r < BORDER_WIDTH)
    x = BORDER_WIDTH + r, vx *= -1, bounces--;
  if (x + r > WIN_W - BORDER_WIDTH)
    x = WIN_W - BORDER_WIDTH - r, vx *= -1, bounces--;
  if (y - r < BORDER_WIDTH)
    y = BORDER_WIDTH + r, vy *= -1, bounces--;
  if (y + r > WIN_H - BORDER_WIDTH)
    y = WIN_H - BORDER_WIDTH - r, vy *= -1, bounces--;

  Collider = {(int)x - AURA - (int)(BULLET_SIZE / 2.0),
              (int)y - AURA - (int)(BULLET_SIZE / 2.0), texSize, texSize};

  if (bounces <= 0)
    alive = false;

  trailTimer += dt;
  while (trailTimer >= TRAIL_INTERVAL) {
    trailTimer -= TRAIL_INTERVAL;
    trail.push_back({x, y});

    while (trail.size() >= TRAIL_SIZE)
      trail.pop_front();
  }
}

void Bullet::render(SDL_Renderer *renderer, SDL_Texture *bulletTex[3]) {
  int texture = std::clamp(2 - bounces + textureIndex, 0, 2);

  for (size_t i = 0; i < trail.size(); i++) {
    auto &seg = trail[i];

    float t =
        trail.size() > 1 ? static_cast<float>(i) / (trail.size() - 1) : 1.0f;

    float expT = pow(t, TRAIL_ALPHA_POWER);

    Uint8 alpha = static_cast<Uint8>(255 * expT);
    SDL_SetTextureAlphaMod(bulletTex[texture], alpha);

    float scale = TRAIL_MIN_SCALE + TRAIL_SCALE_RANGE * expT;
    int scaledSize = static_cast<int>(texSize * scale);

    SDL_Rect dst = {(int)seg.x - scaledSize / 2, (int)seg.y - scaledSize / 2,
                    scaledSize, scaledSize};

    SDL_RenderCopy(renderer, bulletTex[texture], NULL, &dst);
  }

  SDL_SetTextureAlphaMod(bulletTex[texture], 255);

  SDL_RenderCopy(renderer, bulletTex[texture], NULL, &Collider);
}