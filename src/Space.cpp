#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdlib> // rand()
#include <ctime>   // time()
#include <vector>

#include "Constants.h"
#include "UI.h"
#include "Player.h"
#include "Bullet.h"
#include "SDLUtils.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
	SetProcessDPIAware();
#endif

	srand(time(nullptr));
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	TTF_Font *font = nullptr;
	Mix_Music *bgm = nullptr;
	Mix_Chunk *shootSFX = nullptr;
	Mix_Chunk *moveSFX = nullptr;
	SDL_Texture *borderTexture = nullptr;
	SDL_Texture *playerTex = nullptr;
	SDL_Texture *enemyTex = nullptr;
	SDL_Texture *hpTex = nullptr;
	SDL_Texture *bUITex = nullptr;
	SDL_Texture *bulletTex[3] = {nullptr, nullptr, nullptr};

	int engineChannel = 0;
	float currentVol;

	if (!Game::initResources(window, renderer, font, bgm, shootSFX, moveSFX, borderTexture, playerTex, enemyTex, hpTex, bUITex, bulletTex, engineChannel))
		return -1;

	bool running = true;
	SDL_Event event;

	Player player(WIN_W / 2.0f - PLAYER_SIZE / 2.0f, WIN_H / 2.0f - PLAYER_SIZE / 2.0f, playerTex);
	Player replay = player;

	std::vector<Enemy> enemies;
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		float ex = rand() % (WIN_W - (int)ENEMY_SIZE); // X within screen bounds
		float ey = rand() % (WIN_H - (int)ENEMY_SIZE); // Y within screen bounds
		enemies.emplace_back(ex, ey, enemyTex, &player);
	}

	SDL_Color white = {255, 255, 255, 255};
	UI::FPSCounter fpsC(renderer, font, white);
	UI::PlayerHealth pHP(&player, WIN_W - BORDER_WIDTH, BORDER_WIDTH, 30, 5, hpTex);

	UI::BulletUI bUI(BORDER_WIDTH, BORDER_WIDTH, 30, 5, bUITex);

	Uint32 lastTick = SDL_GetTicks();

	std::vector<Bullet> bullets;
	std::vector<EnemyBullet> enemyBullets;
	std::vector<Star> stars;
	bool shoot = true;
	Uint32 bulletInit;

	for (int i = 0; i < STAR_MAX; i++)
		stars.emplace_back(rand() % WIN_W, rand() % WIN_H);

	int mX, mY;

	while (running)
	{
		if (player.src.x > 224 || enemies.size() == 0)
		{
			Game::reset(player, replay, enemies, bullets, enemyBullets, renderer, enemyTex);
			continue;
		}

		Uint32 currentTick = SDL_GetTicks();
		float deltaTime = (currentTick - lastTick) / 1000.0f;
		lastTick = currentTick;

		fpsC.update();

		Game::handleInput(event, running, player, bullets, shoot, bulletInit, shootSFX, mX, mY, currentTick);

		Game::update(deltaTime, player, enemies, bullets, enemyBullets, stars,
					 engineChannel, currentVol, idleVol, moveVol);

		Game::renderSystems(renderer, borderTexture, stars, bullets, enemyBullets, player, enemies, fpsC, bUI, bulletTex, mX, mY, pHP);
	}

	cleanupSDL(window, renderer, font, borderTexture, playerTex, hpTex, bUITex, bulletTex, shootSFX, bgm);
	return 0;
}

/*Shadows And Dust by Scott Buckley | www.scottbuckley.com.au
Music promoted by https://www.chosic.com/free-music/all/
Creative Commons CC BY 4.0
https://creativecommons.org/licenses/by/4.0/*/