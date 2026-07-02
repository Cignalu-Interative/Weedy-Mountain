#pragma once
#include "SDLEncapsulation.h"
#include "Window.h"

namespace SDLEncapsulation
{


	bool SDLEncapsulation::SetTextInputArea(Window* p_window, S_Rect p_rect, int p_cursor)
	{
		if (SDL_SetTextInputArea(p_window->GetWindow(), p_rect, 0) == false)
		{
			SDL_Log("Lib/TextBox.cpp/InitTextBox/SDL_SetTextInputArea non reussie : %s", SDL_GetError());
			return;
		}
		return false;
	}

}
