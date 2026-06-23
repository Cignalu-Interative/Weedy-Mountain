#include "pch.h"
#include "Rectangle.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"
#include "Utils.h"

void Rectangle::Initialize(Vector2f p_position, Vector2i p_dimension, Color p_color)
{
	SetColor(p_color);
	SetDimension(p_dimension);
	SetPosition(p_position);
	m_rect.x = GetPosition({0.0f, 0.0f}).GetX();
	m_rect.y = GetPosition({0.0f, 0.0f}).GetY();
	m_rect.w = GetDimension().GetX();
	m_rect.h = GetDimension().GetY();
}

void Rectangle::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
    if (GetIsVisible() == false)
    {
        return;
    }

    SDL_Renderer* renderer = f_render->m_renderer;

    // Mode bitfield : GUI(1) + Rot(2) + Fill(4)
	// 0 = Monde + NoRot + NoFill, 1 = GUI + NoRot + NoFill, 2 = Monde + Rot + NoFill, 3 = GUI + Rot + NoFill,
    // 4 = Monde + NoRot + Fill, 5 = GUI + NoRot + Fill, 6 = Monde + Rot + Fill, 7 = GUI + Rot + Fill
    uint8_t mode = (m_isGUI ? 1 : 0) | (m_angle != 0.0f ? 2 : 0) | (m_isFilled ? 4 : 0);

    std::array<Vector2f, 4> corners;
    SDL_FRect screenRect;

    // Calcul screen rect selon mode
    if (mode & 1) {  // GUI
        screenRect.x = GetPosition({ 0.0f, 0.0f }).GetX();
        screenRect.y = GetPosition({ 0.0f, 0.0f }).GetY();
        screenRect.w = GetDimension().GetX();
        screenRect.h = GetDimension().GetY();
    }
    else {  // Monde
        if (mode & 2) {  // Rotated
            corners = Utils::GetCorners(GetPosition(), GetDimension(), GetAngle(), GetRotationPoint());
            float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
            for (const auto& corner : corners) {
                Vector2f screen = p_camera->WorldToScreen(corner);
				// minX correspond au coin le plus à gauche, maxX au plus à droite, minY au plus haut, maxY au plus bas
                minX = Utils::LimitMax(minX, screen.GetX());  // std::min
                maxX = Utils::LimitMin(maxX, screen.GetX());  // std::max  
                minY = Utils::LimitMax(minY, screen.GetY());  // std::min
                maxY = Utils::LimitMin(maxY, screen.GetY());  // std::max
            }
            screenRect = { minX, minY, maxX - minX, maxY - minY };
        }
        else {  // AABB
            Vector2f screenPos = p_camera->WorldToScreen(GetPosition({ 0.0f, 0.0f }));
            screenRect.x = screenPos.GetX();
            screenRect.y = screenPos.GetY();
            screenRect.w = GetDimension().GetX() * p_camera->GetZoom();
            screenRect.h = GetDimension().GetY() * p_camera->GetZoom();
        }
    }

    // Culling commun
    Vector2i winSize = f_window->GetWindowSize();
    if (screenRect.w < 1.0f || screenRect.h < 1.0f ||
        screenRect.x > winSize.GetX() || screenRect.y > winSize.GetY() ||
        screenRect.x + screenRect.w < 0 || screenRect.y + screenRect.h < 0)
        return;

    if (SDL_SetRenderDrawColor(renderer, (float)m_color.r, (float)m_color.g, (float)m_color.b, (float)m_color.a) == false)
    {
        SDL_Log("Lib/Rectangle.cpp/Draw/SDL_SetRenderDrawColor/cela na pas fonctionné : %s", SDL_GetError());
		return;
    }

    // RENDU selon 8 cas exacts
	// Les case sont placées dans l'ordre de complexité croissante, pour éviter les if imbriqués et les calculs inutiles (ex: pas besoin de calculer les coins si pas de rotation)
	// exemple : si le mode est GUI + NoRot + Fill, on tombe directement dans le case 5 qui fait un SDL_RenderFillRectF sans calcul de coins ni de WorldToScreen,
    // alors que si on avait fait des if imbriqués, on aurait d'abord vérifié GUI, puis NoRot, puis Fill, et on aurait calculé les coins et fait du WorldToScreen pour rien
    switch (mode) {
    case 7: // GUI + Rot + Fill -> Geometry GUI
    case 3: // GUI + Rot + NoFill -> Lines GUI
        corners = Utils::GetCorners(GetPosition(), GetDimension(), GetAngle(), GetRotationPoint());
        if (mode & 4) {  // Filled
            SDL_Vertex vertices[4];
            for (int i = 0; i < 4; i++) {
                vertices[i].position = { corners[i].GetX(), corners[i].GetY() };
                vertices[i].color = { (float)m_color.r, (float)m_color.g, (float)m_color.b, (float)m_color.a };
                vertices[i].tex_coord = { 0, 0 };
            }
            int indices[] = { 0, 1, 2, 0, 2, 3 };
            if (SDL_RenderGeometry(renderer, nullptr, vertices, 4, indices, 6) == false)
            {
                SDL_Log("Lib/Rectangle.cpp/Draw/SDL_RenderGeometry/cela na pas fonctionné : %s", SDL_GetError());
                return;
            }
        }
        else {  // Lines
            for (int i = 0; i < 4; i++) {
                Vector2f a = corners[i];
                Vector2f b = corners[(i + 1) % 4];
                if (SDL_RenderLine(renderer, a.GetX(), a.GetY(), b.GetX(), b.GetY()) == false)
                {
                    SDL_Log("Lib/Rectangle.cpp/Draw/SDL_RenderLine/cela na pas fonctionné : %s", SDL_GetError());
                    return;
                }
            }
        }
        break;

    case 6: // Monde + Rot + Fill -> Geometry Monde
        for (int i = 0; i < 4; i++) {
            Vector2f screen = p_camera->WorldToScreen(corners[i]);
            screenRect.x = screen.GetX(); screenRect.y = screen.GetY();
            // ... même code Geometry avec WorldToScreen
            SDL_Vertex vertices[4];
            for (int j = 0; j < 4; j++) {
                Vector2f s = p_camera->WorldToScreen(corners[j]);
                vertices[j].position = { s.GetX(), s.GetY() };
                vertices[j].color = {(float)m_color.r, (float)m_color.g, (float)m_color.b, (float)m_color.a};
                vertices[j].tex_coord = { 0, 0 };
            }
            int indices[] = { 0, 1, 2, 0, 2, 3 };
            if (SDL_RenderGeometry(renderer, nullptr, vertices, 4, indices, 6) == false)
            {
                SDL_Log("Lib/Rectangle.cpp/Draw/SDL_RenderGeometry/cela na pas fonctionné : %s", SDL_GetError());
                return;
            }
        }
        break;

    case 2: // Monde + Rot + NoFill -> Lines Monde
        for (int i = 0; i < 4; i++) {
            Vector2f a = p_camera->WorldToScreen(corners[i]);
            Vector2f b = p_camera->WorldToScreen(corners[(i + 1) % 4]);
            if (SDL_RenderLine(renderer, a.GetX(), a.GetY(), b.GetX(), b.GetY()) == false)
            {
                SDL_Log("Lib/Rectangle.cpp/Draw/SDL_RenderLine/cela na pas fonctionné : %s", SDL_GetError());
                return;
            }
        }
        break;

    case 5: // GUI + NoRot + Fill
    case 4: // Monde + NoRot + Fill
        if (SDL_RenderFillRect(renderer, &screenRect) == false)
        {
            SDL_Log("Lib/Rectangle.cpp/Draw/SDL_RenderFillRect/cela na pas fonctionné : %s", SDL_GetError());
            return;
        }
        break;

    case 1: // GUI + NoRot + NoFill
    default: // Monde + NoRot + NoFill
        if (SDL_RenderRect(renderer, &screenRect) == false)
        {
            SDL_Log("Lib/Rectangle.cpp/Draw/SDL_RenderDrawRect/cela na pas fonctionné : %s", SDL_GetError());
            return;
        }
        break;
    }
}