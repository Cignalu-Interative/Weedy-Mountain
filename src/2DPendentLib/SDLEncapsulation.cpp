#include "pch.h"
#include "SDLEncapsulation.h"
#include "Window.h"

namespace SDLEncapsulation
{
	void SetTextInputArea(Window* p_window, S_Rect* p_rect, int p_cursor)
	{
		SDL_Rect sdlRect;
		sdlRect.x = p_rect->x;
		sdlRect.y = p_rect->y;
		sdlRect.w = p_rect->w;
		sdlRect.h = p_rect->h;

		if (SDL_SetTextInputArea(p_window->GetWindow(), &sdlRect, 0) == false)
		{
			SDL_Log("Lib/SDLEncapsulation.cpp/SetTextInputArea/SDL_SetTextInputArea non reussie : %s", SDL_GetError());
			return;
		}
	}






}
