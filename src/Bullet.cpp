#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float startX, float startY,
			   float dirx, float diry,
			   float s)
	: x(startX),
	  y(startY),
	  speed(s),
	  alive(true),
	  trailTimer(0.0f),
	  bounces(2)
{
	vx = dirx * speed;
	vy = diry * speed;
}
EnemyBullet::EnemyBullet(float startX, float startY, float dirx, float diry, float s)
	: Bullet(startX, startY, dirx, diry, s) // call base constructor
{
	bounces = 0; // enemy bullets have fewer bounces
}

void Bullet::update(float dt)
{
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

	Collider = {(int)x - AURA - (int)(BULLET_SIZE / 2.0), (int)y - AURA - (int)(BULLET_SIZE / 2.0), texSize, texSize};

	if (bounces < 0)
		alive = false;

	trailTimer += dt;
	while (trailTimer >= TRAIL_INTERVAL)
	{
		trailTimer -= TRAIL_INTERVAL;
		trail.push_back({x, y});

		while (trail.size() >= TRAIL_SIZE)
			trail.erase(trail.begin());
	}
}

void Bullet::render(SDL_Renderer *renderer, SDL_Texture *bulletTex[3])
{
	for (size_t i = 0; i < trail.size(); i++)
	{
		auto &seg = trail[i];

		float t = trail.size() > 1
					  ? static_cast<float>(i) / (trail.size() - 1)
					  : 1.0f;

		float expT = pow(t, 2.5f);

		Uint8 alpha = static_cast<Uint8>(255 * expT);
		SDL_SetTextureAlphaMod(bulletTex[2 - bounces], alpha);

		float scale = 0.2f + 0.8f * expT;
		int scaledSize =
			static_cast<int>(texSize * scale);

		SDL_Rect dst = {(int)seg.x - scaledSize / 2, (int)seg.y - scaledSize / 2, scaledSize, scaledSize};

		SDL_RenderCopy(renderer, bulletTex[2 - bounces], NULL, &dst);
	}

	SDL_SetTextureAlphaMod(
		bulletTex[2 - bounces], 255);

	SDL_RenderCopy(renderer, bulletTex[2 - bounces], NULL, &Collider);
}