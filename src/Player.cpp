#include "Player.h"
#include "Bullet.h"
#include <cmath>
#include <algorithm>

Entity::Entity(float startX, float startY, SDL_Texture *tex)
	: animTimer(0), texture(tex), x(startX), y(startY), vx(0), vy(0), ax(0), ay(0) { src = {0, 0, SPRITE_FRAME_SIZE, SPRITE_FRAME_SIZE}; }

void Entity::setSize(float size)
{
	s = size;
	Collider = {(int)(x + 0.1 * size), (int)(y + 0.1 * size), (int)(size * 0.8f), (int)(size * 0.8f)};
}

Player::Player(float startX, float startY, SDL_Texture *tex)
	: Entity(startX, startY, tex)
{
	setSize(PLAYER_SIZE);
}

Enemy::Enemy(float startX, float startY, SDL_Texture *tex, Player *player)
	: Entity(startX, startY, tex), p(player)
{
	hp = 1;
	bulletTimer = 0.0f;

	setSize(ENEMY_SIZE);
	dx = p->x + p->s / 2.0f - x - s / 2.0f;
	dy = p->y + p->s / 2.0f - y - s / 2.0f;
}

void Player::update(float dt, bool left, bool right, bool up, bool down)
{
	if (hp <= 0)
	{
		animTimer += dt;
		if (animTimer >= ANIMATION_FRAME_DURATION)
			src.x += SPRITE_FRAME_SIZE, animTimer = 0.0f;
		return;
	}

	if ((left + right) && (up + down))
	{
		vx = (right - left) * SPEED * 0.70710678f;
		vy = -(up - down) * SPEED * 0.70710678f;
	}
	else
	{
		vx = (right - left) * SPEED;
		vy = -(up - down) * SPEED;
	}

	// vx += ax * dt;
	// vy += ay * dt;

	x += vx * dt;
	y += vy * dt;

	x = std::clamp(x, (float)BORDER_WIDTH, (float)(WIN_W - BORDER_WIDTH - s));
	y = std::clamp(y, (float)BORDER_WIDTH, (float)(WIN_H - BORDER_WIDTH - s));

	Collider.x = (int)(x + 0.1 * s);
	Collider.y = (int)(y + 0.1 * s);
}

void Player::render(SDL_Renderer *renderer, float mouseX, float mouseY)
{
	SDL_Rect rect = {(int)x, (int)y, (int)s, (int)s};
	SDL_Point center = {(int)(s / 2), (int)(s / 2)};

	float dx = mouseX - x - s / 2.0f;
	float dy = mouseY - y - s / 2.0f;

	float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90;

	SDL_RenderCopyEx(renderer, texture, &src, &rect, angle, &center, SDL_FLIP_NONE);
}

void Player::shoot(float mX, float mY, std::vector<Bullet> &b)
{
	float dx = mX - x - s / 2;
	float dy = mY - y - s / 2;

	float d = std::sqrt(dx * dx + dy * dy);

	b.emplace_back(x + s / 2, y + s / 2, dx / d, dy / d);
}

void Enemy::update(float dt, std::vector<EnemyBullet> &b)
{
	ay = 0, ax = 0;
	if (hp <= 0) // dead
	{
		animTimer += dt;
		if (animTimer >= ANIMATION_FRAME_DURATION)
			src.x += SPRITE_FRAME_SIZE, animTimer = 0.0f;
		return;
	}

	bulletTimer += dt;
	if (bulletTimer >= 2 * BULLET_DELAY / 1000.0f)
	{
		b.emplace_back(x + s / 2, y + s / 2, dx / d, dy / d);
		bulletTimer = 0;
	}
	if (x <= BORDER_WIDTH)
		vx *= -1;
	if (x + s >= WIN_W - BORDER_WIDTH)
		vx *= -1;
	if (y <= BORDER_WIDTH)
		vy *= -1;
	if (y + s >= WIN_H - BORDER_WIDTH)
		vy *= -1;

	dx = p->x + p->s / 2.0f - x - s / 2.0f;
	dy = p->y + p->s / 2.0f - y - s / 2.0f;
	d = std::sqrt(dx * dx + dy * dy);

	float c = ENEMY_ACCELERATION;

	ay += c * (dx / d);
	ax += -c * (dy / d);

	vx *= std::pow(ENEMY_FRICTION, dt);
	vy *= std::pow(ENEMY_FRICTION, dt);

	vx += ax * dt;
	vy += ay * dt;

	x += vx * dt;
	y += vy * dt;

	x = std::clamp(x, (float)BORDER_WIDTH, (float)(WIN_W - BORDER_WIDTH - s));
	y = std::clamp(y, (float)BORDER_WIDTH, (float)(WIN_H - BORDER_WIDTH - s));

	Collider.x = (int)(x + 0.1 * s);
	Collider.y = (int)(y + 0.1 * s);
}

void Enemy::render(SDL_Renderer *renderer)
{
	SDL_Rect rect = {(int)x, (int)y, (int)s, (int)s};
	SDL_Point center = {(int)(s / 2), (int)(s / 2)};

	float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90;

	SDL_RenderCopyEx(renderer, texture, &src, &rect, angle, &center, SDL_FLIP_NONE);
}
