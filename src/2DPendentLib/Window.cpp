#include "pch.h"
#include "Window.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

Window::~Window()
{
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Window::CreateWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_Init/SDL_Init non reussie : %s", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/IMG_Init/IMG_Init non reussie : %s", IMG_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/TTF_Init/TTF_Init non reussie : %s", TTF_GetError());
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/Mix_OpenAudio/Mix_OpenAudio non reussie : %s", Mix_GetError());
		return false;
	}
	Mix_AllocateChannels(24);

	V_Sync();

	m_windowDimention.SetX((int)(m_displayMode.w / SCREEN_SCALE));
	m_windowDimention.SetY((int)(m_displayMode.h / SCREEN_SCALE));

	m_window = SDL_CreateWindow("Cignalu Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_windowDimention.GetX(),
		m_windowDimention.GetY(),
		SDL_WINDOW_RESIZABLE);

	if (m_window == NULL)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_CreateWindow/SDL_CreateWindow non reussie : %s", SDL_GetError());
		return false;
	}

	if(m_windowDimention.GetX() < m_displayMode.w && m_windowDimention.GetY() < m_displayMode.h)
	{
		m_isFullScreen = false;
	}
	else
	{
		m_isFullScreen = true;
	}

	SDL_Surface* icon = IMG_Load("../../res/2DPendentEngine/Assets/Textures/IconWindow/CignaluEngineIconeV2.png");

	if (icon != nullptr) {
		SDL_SetWindowIcon(m_window, icon);
		SDL_FreeSurface(icon);
	}
	else {
		SDL_Log("Lib/Window.cpp/CreateWindow/IMG_Load/IMG_Load non reussie : %s", IMG_GetError());
		return false;
	}
	return true;
}

int Window::V_Sync()
{
	if (SDL_GetCurrentDisplayMode(0, &m_displayMode) < 0)
	{
		SDL_Log("Lib/Window.cpp/V_Sync/SDL_GetCurrentDisplayMode/SDL_GetCurrentDisplayMode non reussie : %s", SDL_GetError());
	}
	return m_displayMode.refresh_rate;
}

void Window::BoolFullScreen()
{
	Uint32 flags = SDL_GetWindowFlags(m_window);
	if (m_isFullScreen == true)
	{
		if (SDL_SetWindowFullscreen(m_window, 0) < 0)
		{
			SDL_Log("Lib/Window.cpp/BoolFullScreen/SDL_SetWindowFullscreen/SDL_SetWindowFullscreen 1 non reussie : %s", SDL_GetError());
			return;
		}
		m_isFullScreen = false;
	}
	else
	{
		if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			SDL_Log("Lib/Window.cpp/BoolFullScreen/SDL_SetWindowFullscreen/SDL_SetWindowFullscreen 2 non reussie : %s", SDL_GetError());
			return;
		}
		m_isFullScreen = true;
	}
}

void Window::SetWindowSize(Vector2i p_dimention)
{
	SDL_SetWindowSize(m_window, p_dimention.GetX(), p_dimention.GetY());
}

Vector2i Window::GetWindowSize()
{
	SDL_GetWindowSize(m_window, &m_windowDimention.GetX(), &m_windowDimention.GetY());
	Vector2i dimention = { m_windowDimention.GetX(), m_windowDimention.GetY() };
	return dimention;
}

Vector2i Window::GetHalfWindowSize()
{
	Vector2i halfSizewindow = GetWindowSize() / 2;
	return halfSizewindow;
}
