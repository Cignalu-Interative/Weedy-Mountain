#include "pch.h"
#include "Line.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"

Line::Line(Vector2f p_startPoint, Vector2f p_endPoint, Color p_color)
{
	SetPosition(p_startPoint);
	m_endPoint = p_endPoint;
	SetColor(p_color);
}

void Line::SetEndPosition(Vector2f p_endPosition, Vector2f p_anchor)
{
	if (m_dimension.GetX() <= 0 || m_dimension.GetY() <= 0)
	{
		m_endPoint = p_endPosition;
		return;
	}
	else
	{
		m_endPoint.SetX(p_endPosition.GetX() - m_dimension.GetX() * p_anchor.GetX());
		m_endPoint.SetY(p_endPosition.GetY() - m_dimension.GetY() * p_anchor.GetY());
	}
}

void Line::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	if (GetIsVisible() == false)
	{
		return;
	}
	SDL_Renderer* renderer = f_render->m_renderer;
	Vector2f startScreenPos;
	Vector2f endScreenPos;
	if (m_isGUI == true)
	{
		startScreenPos = GetPosition();
		endScreenPos = m_endPoint;
	}
	else
	{
		startScreenPos = p_camera->WorldToScreen(GetPosition());
		endScreenPos = p_camera->WorldToScreen(m_endPoint);
	}

	// Culling : skip si les deux points sont hors écran du même côté
	const float screenW = static_cast<float>(f_window->GetWindowSize().GetX());
	const float screenH = static_cast<float>(f_window->GetWindowSize().GetY());

	const bool bothLeft = startScreenPos.GetX() < 0.0f && endScreenPos.GetX() < 0.0f;
	const bool bothRight = startScreenPos.GetX() > screenW && endScreenPos.GetX() > screenW;
	const bool bothTop = startScreenPos.GetY() < 0.0f && endScreenPos.GetY() < 0.0f;
	const bool bothBottom = startScreenPos.GetY() > screenH && endScreenPos.GetY() > screenH;

	if (bothLeft || bothRight || bothTop || bothBottom)
	{
		return;
	}

	if (SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a) == false)
	{
		SDL_Log("Lib/Line.cpp/Draw/SDL_SetRenderDrawColor/cela na pas fonctionné : %s", SDL_GetError());
		return;
	}
	if (SDL_RenderLine(renderer, startScreenPos.GetX(), startScreenPos.GetY(), endScreenPos.GetX(), endScreenPos.GetY()) == false)
	{
		SDL_Log("Lib/Line.cpp/Draw/SDL_RenderDrawLineF/cela na pas fonctionné : %s", SDL_GetError());
		return;
	}
}