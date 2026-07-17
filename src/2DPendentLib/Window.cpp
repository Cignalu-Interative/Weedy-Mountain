#include "pch.h"
#include "Window.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Utils.h"

Window::~Window()
{
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	/*if (m_glContext)
	{
		SDL_GL_DeleteContext(m_glContext);
		m_glContext = nullptr;
	}*/
	if (m_mixer)
	{
		MIX_DestroyMixer(m_mixer);
		m_mixer = nullptr;
	}
	MIX_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool Window::CreateWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC) == false)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_Init/SDL_Init non reussie : %s", SDL_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/TTF_Init/TTF_Init non reussie : %s", SDL_GetError());
		return false;
	}

	if (MIX_Init() == false)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/Mix_OpenAudio/Mix_OpenAudio non reussie : %s", SDL_GetError());
		return false;
	}

	//SDL_AUDIO_S16LE son classique
	//SDL_AUDIO_F32LE son haut de gamme
	// 44100 Hz est la fréquence d'échantillonnage standard pour l'audio, utilisée dans les CD audio et de nombreux formats audio numériques.
	// 48000 Hz est une fréquence d'échantillonnage couramment utilisée dans la production audio professionnelle, notamment pour les films, la télévision et les jeux vidéo.

	SDL_AudioSpec desiredSpec;
	desiredSpec.format = SDL_AUDIO_F32LE;
	desiredSpec.channels = 6; // 6 pour 5.1 surround sound, 2 pour stereo, 1 pour mono
	desiredSpec.freq = 48000;

	m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desiredSpec);
	if (m_mixer == nullptr)
	{
		SDL_Log("Engine/Window.cpp/Window/CreateWindow/MIX_CreateMixerDevice failed: %s", SDL_GetError());
		return false;
	}

	V_Sync();

	m_windowDimension.SetX((int)(m_displayMode.w / SCREEN_SCALE));
	m_windowDimension.SetY((int)(m_displayMode.h / SCREEN_SCALE));

	m_window = SDL_CreateWindow("Weddy Mountain",
		m_windowDimension.GetX(),
		m_windowDimension.GetY(),
		SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_HIGH_PIXEL_DENSITY
	);

	if (m_window == NULL)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_CreateWindow/SDL_CreateWindow non reussie : %s", SDL_GetError());
		return false;
	}

	if(m_windowDimension.GetX() < m_displayMode.w && m_windowDimension.GetY() < m_displayMode.h)
	{
		m_isFullScreen = false;
	}
	else
	{
		m_isFullScreen = true;
	}

	SDL_Surface* icon = IMG_Load("../../res/2DPendentEngine/Assets/Textures/IconWindow/CignaluEngineIconeV2.png");

	if (icon != nullptr)
	{
		if (SDL_SetWindowIcon(m_window, icon) == false)
		{
			SDL_Log("Lib/Window.cpp/CreateWindow/SDL_SetWindowIcon/SDL_SetWindowIcon non reussie : %s", SDL_GetError());
			return false;
		}
		SDL_DestroySurface(icon);
	}
	else {
		SDL_Log("Lib/Window.cpp/CreateWindow/IMG_Load/IMG_Load non reussie : %s", SDL_GetError());
		return false;
	}
	return true;
}

int Window::V_Sync()
{
	SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
	if (displayID == 0)
	{
		SDL_Log("Lib/Window.cpp/V_Sync/SDL_GetPrimaryDisplay/SDL_GetPrimaryDisplay non reussie : %s", SDL_GetError());
		return -1;
	}

	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displayID);

	if (mode == NULL)
	{
		SDL_Log("Lib/Window.cpp/V_Sync/SDL_GetCurrentDisplayMode/SDL_GetCurrentDisplayMode non reussie : %s", SDL_GetError());
		return -1;
	}

	m_displayMode = *mode;

	return m_displayMode.refresh_rate;
}

void Window::BoolFullScreen()
{
	Uint32 flags = SDL_GetWindowFlags(m_window);
	if (flags & SDL_WINDOW_FULLSCREEN)
	{
		if (SDL_SetWindowFullscreen(m_window, false) == false)
		{
			SDL_Log("Lib/Window.cpp/BoolFullScreen/SDL_SetWindowFullscreen/SDL_SetWindowFullscreen 1 non reussie : %s", SDL_GetError());
			return;
		}
		m_isFullScreen = false;
	}
	else
	{
		if (SDL_SetWindowFullscreen(m_window, true) == false)
		{
			SDL_Log("Lib/Window.cpp/BoolFullScreen/SDL_SetWindowFullscreen/SDL_SetWindowFullscreen 2 non reussie : %s", SDL_GetError());
			return;
		}
		m_isFullScreen = true;
	}
}

void Window::SetWindowSize(Vector2i p_dimention)
{
	if (SDL_SetWindowSize(m_window, p_dimention.GetX(), p_dimention.GetY()) == false)
	{
		SDL_Log("Lib/Window.cpp/SetWindowSize/SDL_SetWindowSize/SDL_SetWindowSize non reussie : %s", SDL_GetError());
		return;
	}
}

Vector2i Window::GetWindowSize()
{
	if (SDL_GetWindowSize(m_window, &m_windowDimension.GetX(), &m_windowDimension.GetY()) == false)
	{
		SDL_Log("Lib/Window.cpp/GetWindowSize/SDL_GetWindowSize/SDL_GetWindowSize non reussie : %s", SDL_GetError());
		return { 0, 0 };
	}
	Vector2i dimention = { m_windowDimension.GetX(), m_windowDimension.GetY() };
	return dimention;
}

Vector2i Window::GetHalfWindowSize()
{
	Vector2i halfSizewindow = GetWindowSize() / 2;
	return halfSizewindow;
}

void Window::SetWindowAlwaysOnTop(bool p_alwaysOnTop)
{
	if (SDL_SetWindowAlwaysOnTop(m_window, p_alwaysOnTop) == false)
	{
		SDL_Log("Lib/Window.cpp/SetWindowAlwaysOnTop/SDL_SetWindowAlwaysOnTop/SDL_SetWindowAlwaysOnTop non reussie : %s", SDL_GetError());
		return;
	}
}

void Window::SetWindowOpacity(float p_opacity)
{
	 float oppacity = Utils::Clamp(p_opacity, 0.0f, 1.0f);
	if (SDL_SetWindowOpacity(m_window, oppacity) == false)
	{
		SDL_Log("Lib/Window.cpp/SetWindowOpacity/SDL_SetWindowOpacity/SDL_SetWindowOpacity non reussie : %s", SDL_GetError());
		return;
	}
}

void Window::SetFlashWindow(int p_flash)
{
	switch (p_flash)
	{
	case 0:
		if (SDL_FlashWindow(m_window, SDL_FLASH_CANCEL) == false)
		{
			SDL_Log("Lib/Window.cpp/SetFlashWindow/SDL_FlashWindow/SDL_FlashWindow non reussie : %s", SDL_GetError());
			return;
		}
		break;
	case 1:
		if (SDL_FlashWindow(m_window, SDL_FLASH_BRIEFLY) == false)
		{
			SDL_Log("Lib/Window.cpp/SetFlashWindow/SDL_FlashWindow/SDL_FlashWindow non reussie : %s", SDL_GetError());
			return;
		}
		break;
	case 2:
		if (SDL_FlashWindow(m_window, SDL_FLASH_UNTIL_FOCUSED) == false)
		{
			SDL_Log("Lib/Window.cpp/SetFlashWindow/SDL_FlashWindow/SDL_FlashWindow non reussie : %s", SDL_GetError());
			return;
		}
		break;
	}
}
