#pragma once

#include "Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "Player.h"

class Player;

namespace UI
{
class FPSCounter
{
  public:
	FPSCounter(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);
	~FPSCounter();

	void update();

	void render();

private:
	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Color color;
	int frames;
	float fps;
	Uint32 lastTick;
	SDL_Texture *texture;
	SDL_Rect rect;
};

class BulletUI
{
  public:
	BulletUI(int startX, int startY, int s, int g, SDL_Texture* tex);

	void render(SDL_Renderer *renderer,
				int activeCount);

  private:
	int x, y, size, gap;
	int bullets;
	SDL_Texture *texture;
	SDL_Rect rect;
	SDL_Rect temp;
};

class PlayerHealth
{
  public:
	PlayerHealth(Player *player, int startX, int startY, int s, int g, SDL_Texture* tex);
	
	//void update();
	void render(SDL_Renderer *renderer);
		
  private:
	int x, y, size, gap;
	int hp;
	Player *p;
	SDL_Texture *texture;
	SDL_Rect rect;
	SDL_Rect temp;
};

}; // namespace UI