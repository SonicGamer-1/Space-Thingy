#pragma once

#include "Constants.h"
#include <SDL2/SDL.h>
#include <vector>

class Bullet;
class EnemyBullet; // forward declaration

class Entity
{
  protected:
	float animTimer;
	SDL_Texture *texture;

  public:
	float x, y;
	float vx, vy;
	float ax, ay;
	float s;
	int hp = PLAYER_MAX_HEALTH;

	SDL_Rect src;
	SDL_Rect Collider;
	
	Entity(float startX, float startY, SDL_Texture* tex);

	void setSize(float size);
};

class Player : public Entity
{
  public:
	Player(float startX, float startY, SDL_Texture *tex);

	void update(float dt, bool left, bool right, bool up, bool down);
	void render(SDL_Renderer *renderer, float mouseX, float mouseY);
	void shoot(float mX, float mY, std::vector<Bullet> &b);
};

class Enemy : public Entity
{
	Player *p;
	float dx, dy, d;
	float bulletTimer;

  public:
	Enemy(float startX, float startY, SDL_Texture *tex, Player *player);

	void update(float dt, std::vector<EnemyBullet> &b);
	void render(SDL_Renderer *renderer);
};