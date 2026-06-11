#include "pch.h"
#include "Render.h"
#include "Window.h"

Render::Render()
{
	SetBackgroundColor(ColorsNamed::Black);
}

Render::~Render()
{
	SDL_DestroyRenderer(m_renderer);
}

bool Render::CreateRenderer(Window& f_window)
{
	m_renderer = SDL_CreateRenderer(f_window.m_window, NULL, SDL_RENDERER_ACCELERATED);
	if (m_renderer == NULL)
	{
		SDL_Log("Lib/Render.cpp/CreateRenderer/SDL_CreateRenderer/SDL_CreateRenderer non reussie : %s", SDL_GetError());
		return false;
	}
	return true;
}

void Render::ClearRenderer()
{
	if (SDL_SetRenderDrawColor(m_renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a) < 0)
	{
		SDL_Log("Lib/Render.cpp/ClearRenderer/SDL_SetRenderDrawColor/SDL_SetRenderDrawColor non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_RenderClear(m_renderer) < 0)
	{
		SDL_Log("Lib/Render.cpp/ClearRenderer/SDL_RenderClear/SDL_RenderClear non reussie : %s", SDL_GetError());
		return;
	}
}

void Render::PresentRenderer()
{
	SDL_RenderPresent(m_renderer);
}

void Render::ClearOpenGL()
{
	glClearColor(m_OpenGLColor.r, m_OpenGLColor.g, m_OpenGLColor.b, m_OpenGLColor.a);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Render::RenderOpenGL(Window* f_window)
{
	SDL_GL_SwapWindow(f_window->GetWindow());
}

void Render::SetBackgroundColor(Color p_backgroundColor)
{
	m_backgroundColor = p_backgroundColor;

	m_OpenGLColor = glm::vec4(
		m_backgroundColor.r / 255.f,
		m_backgroundColor.g / 255.f,
		m_backgroundColor.b / 255.f,
		m_backgroundColor.a / 255.f
	);
}
