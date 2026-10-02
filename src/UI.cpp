#include "UI.h"

namespace UI
{
FPSCounter::FPSCounter(SDL_Renderer *rend, TTF_Font *fon, SDL_Color col)
	: renderer(rend), font(fon), color(col), frames(0), fps(0.0f), lastTick(SDL_GetTicks()), texture(nullptr) {}

FPSCounter::~FPSCounter()
{
	if (texture)
		SDL_DestroyTexture(texture);
}

void FPSCounter::update()
{
	frames++;
	Uint32 now = SDL_GetTicks();

	if (now - lastTick >= 1000)
	{
		fps = frames / ((now - lastTick) / 1000.0f);
		frames = 0;
		lastTick = now;

		if (texture)
			SDL_DestroyTexture(texture);

		std::string fpsText =
			"FPS: " + std::to_string((int)fps);

		SDL_Surface *surf = TTF_RenderText_Solid(font, fpsText.c_str(), color);

		texture = SDL_CreateTextureFromSurface(renderer, surf);

		rect = {BORDER_WIDTH, WIN_H - surf->h -BORDER_WIDTH, surf->w, surf->h};

		SDL_FreeSurface(surf);
	}
}

void FPSCounter::render()
{
	if (texture)
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

BulletUI::BulletUI(int startX, int startY, int s, int g, SDL_Texture *tex)
	: x(startX), y(startY), size(s), gap(g), texture(tex) { rect = {x + gap, y + gap, size / 2, size}; }

void BulletUI::render(SDL_Renderer *renderer, int activeCount)
{
	temp = rect;

	for (int i = 0; i < activeCount; i++)
	{
		SDL_RenderCopy(renderer, texture, nullptr, &temp);
		temp.x += size / 2 + gap;
	}
}

PlayerHealth::PlayerHealth(Player *player, int startX, int startY, int s, int g, SDL_Texture *tex)
	: p(player), x(startX), y(startY), size(s), gap(g), texture(tex) { rect = {x, y + gap, size, size}; }

//void PlayerHealth::update(){}
void PlayerHealth::render(SDL_Renderer *renderer)
{
	temp = rect;
	hp = p->hp;

	for (int i = 0; i < (int)(hp / 5); i++)
	{
		temp.x -= size + gap;
		SDL_RenderCopy(renderer, texture, nullptr, &temp);
	}

	if (hp % 5 > 0)
	{
		float frac = (hp % 5) / 5.0f;
		frac = 1 - frac;
		int visibleWidth = (int)(16 * frac);

		SDL_Rect src = {visibleWidth, 0, 16 - visibleWidth, 16};

		temp.x -= size * (1 - frac) + gap;
		temp.w = size * (1 - frac);
		temp.h = size;

		SDL_RenderCopy(renderer, texture, &src, &temp);
	}

} //hp=p.hp sdl font

}; // namespace UI