#include "SDLUtils.h"
#include <algorithm>
#include "../assets/SpaceShip.h"
#include "../assets/Roboto-Regular.h"
#include "../assets/BulletSounds.h"
#include "../assets/PlayerSounds.h"
#include "../assets/uiTex.h"

static TTF_Font *loadFontFromMemory(unsigned char *data, unsigned int size, int fontSize);

SDL_Sensor *accel = nullptr; // Global

bool initSDL(SDL_Window *&window, SDL_Renderer *&renderer, TTF_Font *&font,
			 unsigned char *fontData, unsigned int fontDataLen, int fontSize)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return false;

	if (IMG_Init(IMG_INIT_PNG) == 0)
		return false;

	if (TTF_Init() == -1)
		return false;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		return false;

	font = loadFontFromMemory(fontData, fontDataLen, fontSize);
	if (!font)
		return false;

	window = SDL_CreateWindow("Space Thingy",
							  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  WIN_W, WIN_H,
							  SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (!window)
		return false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		return false;

	SDL_RenderSetLogicalSize(renderer, WIN_W, WIN_H);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RaiseWindow(window);
	SDL_SetWindowInputFocus(window);

	SDL_InitSubSystem(SDL_INIT_SENSOR);

	for (int i = 0; i < SDL_NumSensors(); i++)
	{
		if (SDL_SensorGetDeviceType(i) == SDL_SENSOR_ACCEL)
		{
			accel = SDL_SensorOpen(i);
			break;
		}
	}

	return true;
}

void cleanupSDL(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *borderTexture, SDL_Texture *playerTex, SDL_Texture *hpTex, SDL_Texture *bUITex, SDL_Texture *bulletTex[3], Mix_Chunk *shootSFX, Mix_Music *bgm)
{
	SDL_DestroyTexture(borderTexture);
	SDL_DestroyTexture(playerTex);
	SDL_DestroyTexture(hpTex);
	SDL_DestroyTexture(bUITex);
	for (int i = 0; i < 3; i++)
		SDL_DestroyTexture(bulletTex[i]);

	SDL_SensorClose(accel);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(font);
	Mix_FreeChunk(shootSFX);
	Mix_FreeMusic(bgm);

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture *createBorderTexture(SDL_Renderer *renderer, int width, int height, int thickness)
{
	SDL_Surface *surf = SDL_CreateRGBSurface(0, width, height, 32,
											 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (!surf)
		return nullptr;

	SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, 0, 0, 0, 0)); // transparent

	Uint32 white = SDL_MapRGBA(surf->format, 255, 255, 255, 255);
	for (int i = 0; i < thickness; i++)
	{
		SDL_Rect r;
		r = {i, i, width - i * 2, 1};
		SDL_FillRect(surf, &r, white); // top
		r = {i, height - i - 1, width - i * 2, 1};
		SDL_FillRect(surf, &r, white); // bottom
		r = {i, i, 1, height - i * 2};
		SDL_FillRect(surf, &r, white); // left
		r = {width - i - 1, i, 1, height - i * 2};
		SDL_FillRect(surf, &r, white); // right
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	if (texture)
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	return texture;
}

SDL_Texture *createBulletTexture(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b)
{
	int texW = BULLET_SIZE + 2 * AURA;
	int texH = BULLET_SIZE + 2 * AURA;

	SDL_Surface *surf = SDL_CreateRGBSurface(0, texW, texH, 32,
											 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (!surf)
		return nullptr;

	SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, 0, 0, 0, 0));

	SDL_Rect glowRect = {0, 0, texW, texH};
	SDL_FillRect(surf, &glowRect, SDL_MapRGBA(surf->format, 255, 255, 255, 128));

	SDL_Rect mainRect = {AURA, AURA, BULLET_SIZE, BULLET_SIZE};
	SDL_FillRect(surf, &mainRect, SDL_MapRGBA(surf->format, r, g, b, 255));

	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	if (tex)
		SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

	return tex;
}

static SDL_Texture *loadTextureFromMemory(SDL_Renderer *renderer, unsigned char *data, unsigned int size)
{
	SDL_RWops *rw = SDL_RWFromConstMem(data, size);
	if (!rw)
		return nullptr;

	SDL_Surface *surface = IMG_Load_RW(rw, 1);
	if (!surface)
		return nullptr;

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

static TTF_Font *loadFontFromMemory(unsigned char *data, unsigned int size, int fontSize)
{
	SDL_RWops *rw = SDL_RWFromConstMem(data, size);
	if (!rw)
		return nullptr;

	TTF_Font *font = TTF_OpenFontRW(rw, 1, fontSize); // auto-frees RWops
	return font;
}

static Mix_Chunk *loadSoundFromMemory(unsigned char *data, unsigned int size)
{
	SDL_RWops *rw = SDL_RWFromConstMem(data, size);
	if (!rw)
		return nullptr;

	Mix_Chunk *chunk = Mix_LoadWAV_RW(rw, 1); // auto-frees RWops
	return chunk;
}

void loadAudio(Mix_Music *&bgm, Mix_Chunk *&shootSFX, Mix_Chunk *&moveSFX, int &engineChannel)
{
	// Load BGM
	bgm = Mix_LoadMUS("assets/BGM.ogg");
	if (!bgm)
		bgm = Mix_LoadMUS("BGM.ogg");
	if (bgm)
		Mix_VolumeMusic(bgmVol);
	Mix_PlayMusic(bgm, -1); // loop forever

	// Load SFX
	shootSFX = loadSoundFromMemory(ShootSFX, ShootSFX_len);
	moveSFX = loadSoundFromMemory(ShipMove, ShipMove_len);

	// Allocate channels (only what you need)
	Mix_AllocateChannels(8);

	// Engine channel setup
	engineChannel = 0;
	Mix_PlayChannel(engineChannel, moveSFX, -1); // loop forever
	Mix_Volume(engineChannel, idleVol);			 // start at idle volume
}

void loadTextures(SDL_Renderer *renderer, SDL_Texture *&borderTexture, SDL_Texture *&playerTex, SDL_Texture *&enemyTex, SDL_Texture *&hpTex, SDL_Texture *&bUITex, SDL_Texture *bulletTex[3])
{
	borderTexture = createBorderTexture(renderer, WIN_W, WIN_H, BORDER_WIDTH);
	playerTex = loadTextureFromMemory(renderer, ship, ship_len);
	enemyTex = loadTextureFromMemory(renderer, enemyttt, enemy_len);

	hpTex = loadTextureFromMemory(renderer, hpttt, hpttt_len);
	bUITex = loadTextureFromMemory(renderer, bullettt, bullettt_len);

	bulletTex[0] = createBulletTexture(renderer, 0, 255, 0);
	bulletTex[1] = createBulletTexture(renderer, 255, 255, 0);
	bulletTex[2] = createBulletTexture(renderer, 255, 0, 0);
}

namespace Game
{
	void engineSFX(Player &player, int engineChannel, float &currentVol, float idleVol, float moveVol, float deltaTime)
	{
		float targetVol = (std::abs(player.vx) + std::abs(player.vy)) ? moveVol : idleVol;
		currentVol += (targetVol - currentVol) * ENGINE_VOLUME_ADJUSTMENT_SPEED * deltaTime;
		Mix_Volume(engineChannel, (int)currentVol);
	}

	bool initResources(SDL_Window *&window, SDL_Renderer *&renderer, TTF_Font *&font, Mix_Music *&bgm, Mix_Chunk *&shootSFX, Mix_Chunk *&moveSFX, SDL_Texture *&borderTexture, SDL_Texture *&playerTex, SDL_Texture *&enemyTex, SDL_Texture *&hpTex, SDL_Texture *&bUITex, SDL_Texture *bulletTex[3], int &engineChannel)
	{
		if (!initSDL(window, renderer, font, Roboto_Regular, Roboto_Regular_len, DEFAULT_FONT_SIZE))
			return false;

		loadAudio(bgm, shootSFX, moveSFX, engineChannel);

		loadTextures(renderer, borderTexture, playerTex, enemyTex, hpTex, bUITex, bulletTex);

		return true;
	}

	void handleInput(SDL_Event &event, bool &running, Player &player, std::vector<Bullet> &bullets, bool &shoot, Uint32 &bulletInit, Mix_Chunk *shootSFX, int &mX, int &mY, Uint32 currentTick)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;

			if (event.type == SDL_MOUSEMOTION)
			{
				mX = event.motion.x;
				mY = event.motion.y;
			}

			if (event.button.button == SDL_BUTTON_LEFT && bullets.size() < MAX_BULLETS && shoot)
			{
				shoot = false;
				bulletInit = currentTick;

				Mix_PlayChannel(-1, shootSFX, 0);
				player.shoot(mX, mY, bullets);
			}
		}

		if (!shoot && (currentTick - bulletInit) > BULLET_DELAY / 3)
			shoot = true;
	}

	void update(float deltaTime, Player &player, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, std::vector<EnemyBullet> &enemyBullets, std::vector<Star> &stars, int engineChannel, float &currentVol, float idleVol, float moveVol)
	{
		bool u = 0, d = 0, l = 0, r = 0;

		float data[3];

		if (accel && SDL_SensorGetData(accel, data, 3) == 0)
		{
			float x = data[1];
			float y = data[0];

			if (x < -1)
				r = true;
			else if (x > 1)
				l = true;
			else
				l = 0, r = 0;

			if (y < -1)
				u = true;
			else if (y > 1)
				d = true;
			else
				u = 0, d = 0;
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		player.update(deltaTime,
					  keys[SDL_SCANCODE_A] + r,
					  keys[SDL_SCANCODE_D] + l,
					  keys[SDL_SCANCODE_W] + u,
					  keys[SDL_SCANCODE_S] + d);

		for (auto &e : enemies)
			e.update(deltaTime, enemyBullets);

		engineSFX(player, engineChannel, currentVol, idleVol, moveVol, deltaTime);

		for (auto &s : stars)
			s.update(deltaTime);

		for (auto &b : bullets)
		{
			b.update(deltaTime);
			for (auto &e : enemies)
			{
				if (SDL_HasIntersection(&b.Collider, &e.Collider))
				{
					b.alive = false;
					e.hp--;
					break;
				}
			}
		}

		for (auto &b : enemyBullets)
		{
			b.update(deltaTime);
			if (SDL_HasIntersection(&b.Collider, &player.Collider))
			{
				b.alive = false;
				player.hp--;
			}
		}

		enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
									 [](Enemy &e)
									 { return e.src.x > 224; }),
					  enemies.end());

		bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
									 [](Bullet &b)
									 { return !b.alive; }),
					  bullets.end());

		enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
										  [](EnemyBullet &b)
										  { return !b.alive; }),
						   enemyBullets.end());
	}

	void renderSystems(SDL_Renderer *renderer, SDL_Texture *borderTexture, std::vector<Star> &stars, std::vector<Bullet> &bullets, std::vector<EnemyBullet> &enemyBullets, Player &player, std::vector<Enemy> &enemies, UI::FPSCounter &fpsC, UI::BulletUI &bUI, SDL_Texture *bulletTex[3], int mX, int mY, UI::PlayerHealth &hp)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// draw border texture
		SDL_RenderCopy(renderer, borderTexture, NULL, NULL);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // yellow star
		for (auto &s : stars)
			s.render(renderer);

		for (auto &b : bullets)
			b.render(renderer, bulletTex);
		for (auto &b : enemyBullets)
			b.render(renderer, bulletTex);

		// draw player
		player.render(renderer, mX, mY);
		for (auto &e : enemies)
			e.render(renderer);

		fpsC.render();
		hp.render(renderer);
		bUI.render(renderer, MAX_BULLETS - bullets.size());

		SDL_RenderPresent(renderer);
	}

	void reset(Player &player, Player &replay, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, std::vector<EnemyBullet> &enemyBullets, SDL_Renderer *&renderer, SDL_Texture *&enemyTex)
	{
		if (player.hp <= 0)
			SDL_SetRenderDrawColor(renderer, 200, 20, 0, 255);

		else if (enemies.size() == 0)
			SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

		player = replay;

		enemies.clear();
		bullets.clear();
		enemyBullets.clear();

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(GAME_RESET_PAUSE_MS);

		// Respawn enemies
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			float ex = rand() % (WIN_W - (int)ENEMY_SIZE);
			float ey = rand() % (WIN_H - (int)ENEMY_SIZE);

			enemies.emplace_back(ex, ey, enemyTex, &player);
		}
	}
}; // namespace Game