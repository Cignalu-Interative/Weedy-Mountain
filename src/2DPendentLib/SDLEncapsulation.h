#pragma once
#include "SDL3/SDL.h"

class Window;

namespace SDLEncapsulation
{
	struct S_Rect
	{
		int x = 0, y = 0, w = 0, h = 0;
	};

	void SetTextInputArea(Window* p_window, S_Rect* p_rect, int p_cursor);































}
