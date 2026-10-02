#pragma once

#include "Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <algorithm>



#include "Player.h"
#include "Bullet.h"
#include "UI.h"

struct Star
{
	float x, y;

	Star(float startX, float startY)
	{
		x = startX;
		y = startY;
	}

	void update(float dt)
	{
		// random movement between -1 and 1
		float dx = (rand() % 3 - 1);
		float dy = (rand() % 3 - 1);

		x += dx * 100.0f * dt;
		y += dy * 100.0f * dt;

		x = std::clamp(x, 0.0f, (float)WIN_W);
		y = std::clamp(y, 0.0f, (float)WIN_H);
	}

	void render(SDL_Renderer *renderer)
	{
		SDL_RenderDrawPoint(renderer, (int)x, (int)y);
	}
};

SDL_Texture *CreateBorderTexture(SDL_Renderer *renderer, int width, int height, int thickness);
SDL_Texture *createBulletTexture(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);

static SDL_Texture *loadTextureFromMemory(SDL_Renderer *renderer, unsigned char *data, int size);
static TTF_Font *loadFontFromMemory(unsigned char *data, unsigned int size, int fontSize);
static Mix_Chunk *loadSoundFromMemory(unsigned char *data, unsigned int size);
static Mix_Music *loadMusicFromMemory(unsigned char *data, unsigned int size);

SDL_Texture *CreateBorderTexture(SDL_Renderer *renderer, int width, int height, int thickness);
SDL_Texture *createBulletTexture(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);

bool initSDL(SDL_Window *&window, SDL_Renderer *&renderer, TTF_Font *&font, unsigned char *fontData, unsigned int fontDataLen, int fontSize);
void loadAudio(Mix_Music *&bgm, Mix_Chunk *&shootSFX, Mix_Chunk *&moveSFX, int &engineChannel);
void loadTextures(SDL_Renderer *renderer, SDL_Texture *&borderTexture, SDL_Texture *&playerTex, SDL_Texture *&enemyTex, SDL_Texture *&hpTex, SDL_Texture *&bUITex, SDL_Texture *bulletTex[3]);
void cleanupSDL(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *borderTexture, SDL_Texture *playerTex, SDL_Texture *hpTex, SDL_Texture *bUITex, SDL_Texture *bulletTex[3], Mix_Chunk *shootSFX, Mix_Music *bgm);

namespace Game
{
void engineSFX(Player &player, int engineChannel, float &currentVol, float idleVol, float moveVol, float deltaTime);

bool initResources(SDL_Window *&window, SDL_Renderer *&renderer, TTF_Font *&font, Mix_Music *&bgm, Mix_Chunk *&shootSFX, Mix_Chunk *&moveSFX, SDL_Texture *&borderTexture, SDL_Texture *&playerTex, SDL_Texture *&enemyTex, SDL_Texture *&hpTex, SDL_Texture *&bUITex, SDL_Texture *bulletTex[3], int &engineChannel);

void handleInput(SDL_Event &event, bool &running, Player &player, std::vector<Bullet> &bullets, bool &shoot, Uint32 &bulletInit, Mix_Chunk *shootSFX, int &mX, int &mY, Uint32 currentTick);

void update(float deltaTime, Player &player, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, std::vector<EnemyBullet> &enemyBullets, std::vector<Star> &stars, int engineChannel, float &currentVol, float idleVol, float moveVol);

void renderSystems(SDL_Renderer *renderer, SDL_Texture *borderTexture, std::vector<Star> &stars, std::vector<Bullet> &bullets, std::vector<EnemyBullet> &enemyBullets, Player &player, std::vector<Enemy> &enemies, UI::FPSCounter &fpsC, UI::BulletUI &bUI, SDL_Texture *bulletTex[3], int mX, int mY, UI::PlayerHealth &hp);

void reset(Player &player, Player &replay, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, std::vector<EnemyBullet> &enemyBullets, SDL_Renderer *&renderer, SDL_Texture *&enemyTex);
};