#pragma once
#include "SDL3/SDL.h"

class Window;

namespace SDLEncapsulation
{
	static struct S_Rect
	{
		int x, y, w, h;
	};

	static bool SetTextInputArea(Window* p_window, S_Rect p_rect, int p_cursor);































}
