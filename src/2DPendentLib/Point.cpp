#include "pch.h"
#include "Point.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"

Point::Point(Vector2f p_position, Color p_color)
{
	SetPosition(p_position);
	SetColor(p_color);
}

void Point::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	if (GetIsVisible() == false)
	{
		return;
	}
	SDL_Renderer* renderer = f_render->m_renderer;
	Vector2f screenPos;
	if (m_isGUI == true)
	{
		screenPos = GetPosition({ 0.0f, 0.0f });
	}
	else
	{
		screenPos = p_camera->WorldToScreen(GetPosition());
	}

	Vector2i dimensionWindow = f_window->GetWindowSize();

	if (screenPos.GetX() < 0 || screenPos.GetX() > dimensionWindow.GetX() ||
		screenPos.GetY() < 0 || screenPos.GetY() > dimensionWindow.GetY())
	{
		//std::cerr << "no draw" << std::endl;
		return;
	}
	if (SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a) == false)
	{
		SDL_Log("Lib/Point.cpp/Draw/SDL_SetRenderDrawColor/cela na pas fonctionné : %s", SDL_GetError());
		return;
	}
	if (SDL_RenderPoint(renderer, screenPos.GetX(), screenPos.GetY()) == false)
	{
		SDL_Log("Lib/Point.cpp/Draw/SDL_RenderDrawPointF/cela na pas fonctionné : %s", SDL_GetError());
		return;
	}
	//SDL_RenderDrawPointF(renderer, m_position.GetX(), m_position.GetY());
}
