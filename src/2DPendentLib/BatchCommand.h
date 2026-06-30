#pragma once
#include <SDL3/SDL.h>

struct TileTiledCommand
{
	SDL_Texture* s_texture = nullptr;
	SDL_FRect s_srcRect{};
	SDL_FRect s_dstRect{};
};

struct SpriteCommand
{
	SDL_Texture* s_texture = nullptr;
	SDL_FRect s_srcRect{};
	SDL_FRect s_dstRect{};
};
