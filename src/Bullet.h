#pragma once

#include "Constants.h"
#include <SDL2/SDL.h>
#include <vector>

class Bullet
{
  protected:
	int texSize = BULLET_SIZE + 2 * AURA;

  public:
	struct TrailSegment
	{
		float x, y;
	};

	float x, y;
	float vx, vy;
	float speed;
	bool alive;
	int bounces;

	SDL_Rect Collider;

	std::vector<TrailSegment> trail;
	float trailTimer;

	Bullet() : x(0), y(0), vx(0), vy(0), alive(false) {}

	Bullet(float startX, float startY,
		   float dirx, float diry,
		   float s = BULLET_SPEED);

	virtual void update(float dt);
	void render(SDL_Renderer *renderer,
				SDL_Texture *bulletTex[3]);
	virtual ~Bullet() {}
};

class EnemyBullet : public Bullet
{
  public:
	EnemyBullet(float startX, float startY, float dirx, float diry, float s = BULLET_SPEED);
};