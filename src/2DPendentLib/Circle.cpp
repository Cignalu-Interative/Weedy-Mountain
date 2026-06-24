#include "pch.h"
#include "Circle.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"
#include "Utils.h"

void Circle::Initialize(Vector2f p_position, Vector2i p_dimension, Color p_color)
{
	SetColor(p_color);
	SetDimension(p_dimension);
	SetPosition(p_position);
}

void Circle::RebuildCache(float p_rx, float p_ry)
{
    m_cachedRx = p_rx;
    m_cachedRy = p_ry;

    const int iry = (int)p_ry;
    m_scanlineCache.resize(iry + 1);

    for (int dy = 0; dy <= iry; dy++)
    {
        const float ratio = (float)dy / p_ry;
        m_scanlineCache[dy] = p_rx * sqrtf(1.0f - ratio * ratio);
    }
}

float Circle::SampleCache(float p_localY) const
{
    const float absY = p_localY < 0.0f ? -p_localY : p_localY; 
    const int   idx = (int)absY;
    const float frac = absY - (float)idx;

    if (idx + 1 < (int)m_scanlineCache.size())
    {
        return m_scanlineCache[idx] * (1.0f - frac) + m_scanlineCache[idx + 1] * frac;
    }
    return m_scanlineCache[idx];
}

void Circle::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
    if (GetIsVisible() == false)
    {
        return;
    }

    float rx, ry;
    Vector2f centerPos;
    if (m_isGUI == true)
    {
        rx = GetDimension().GetX() / 2.0f;
        ry = GetDimension().GetY() / 2.0f;
        centerPos = GetPosition();
    }
    else
    {
        const float zoom = p_camera->GetZoom();
        rx = (GetDimension().GetX() / 2.0f) * zoom;
        ry = (GetDimension().GetY() / 2.0f) * zoom;
        centerPos = p_camera->WorldToScreen(GetPosition());
    }

    const float rotation = GetAngle() * Utils::DEG2RAD;
    const float cosR = Utils::Cos(rotation);
    const float sinR = Utils::Sin(rotation);

    const std::array<Vector2f, 4> corners = Utils::GetCorners(centerPos, GetDimension(), GetAngle(),GetRotationPoint());

    float minX = corners[0].GetX(), maxX = corners[0].GetX();
    float minY = corners[0].GetY(), maxY = corners[0].GetY();

    for (int i = 1; i < 4; i++)
    {
        if (corners[i].GetX() < minX) minX = corners[i].GetX();
        if (corners[i].GetX() > maxX) maxX = corners[i].GetX();
        if (corners[i].GetY() < minY) minY = corners[i].GetY();
        if (corners[i].GetY() > maxY) maxY = corners[i].GetY();
    }
    const Vector2i dimensionWindow = f_window->GetWindowSize();
    if (maxX < 0.0f || minX > dimensionWindow.GetX() ||
        maxY < 0.0f || minY > dimensionWindow.GetY())
    {
        return;
    }

    const float cx = centerPos.GetX();
    const float cy = centerPos.GetY();

    SDL_Renderer* renderer = f_render->m_renderer;

    if (SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a) == false)
    {
        SDL_Log("Lib/Circle.cpp/Draw/SDL_SetRenderDrawColor/cela na pas fonctionné : %s", SDL_GetError());
        return;
    }

    if (m_isFilled == true)
    {
        const int segments = m_precision;
        const float step = (2.0f * Utils::PI) / segments;

        std::vector<SDL_Vertex> vertices;
        std::vector<int> indices;
        vertices.reserve(segments + 1);
        indices.reserve(segments * 3);

        SDL_Vertex center;
        center.position = { cx, cy };
        center.color = {(float)m_color.r / 255.f, (float)m_color.g / 255.f, (float)m_color.b /255.f, (float)m_color.a / 255.f };
        center.tex_coord = { 0.0f, 0.0f };
        vertices.push_back(center);

        // Points du contour
        for (int i = 0; i <= segments; i++)
        {
            const float angle = i * step;
            const float localX = rx * cosf(angle);
            const float localY = ry * sinf(angle);

            const float screenX = cx + localX * cosR - localY * sinR;
            const float screenY = cy + localX * sinR + localY * cosR;

            SDL_Vertex v;
            v.position = { screenX, screenY };
            v.color = { (float)m_color.r / 255.f, (float)m_color.g / 255.f, (float)m_color.b / 255.f, (float)m_color.a / 255.f };
            v.tex_coord = { 0.0f, 0.0f };
            vertices.push_back(v);
        }

        // Triangles : centre + deux points consécutifs du contour
        for (int i = 0; i < segments; i++)
        {
            indices.push_back(0);       // centre
            indices.push_back(i + 1);   // point i
            indices.push_back(i + 2);   // point i+1
        }

        if (SDL_RenderGeometry(renderer,
            nullptr,
            vertices.data(), (int)vertices.size(),
            indices.data(), (int)indices.size()) == false)
        {
            SDL_Log("Lib/Circle.cpp/Draw/SDL_RenderGeometry : %s", SDL_GetError());
            return;
        }
    }

    const float step = (2.0f * Utils::PI) / m_precision;
    int px1 = (int)(cx + rx * cosR);
    int py1 = (int)(cy + rx * sinR);
    for (int i = 0; i <= m_precision; i++)
    {
        const float angle = (i + 1) * step;
        const float localX = rx * Utils::Cos(angle);
        const float localY = ry * Utils::Sin(angle);
        const int   px2 = (int)(cx + localX * cosR - localY * sinR);
        const int   py2 = (int)(cy + localX * sinR + localY * cosR);
        if (SDL_RenderLine(renderer, px1, py1, px2, py2) == false)
        {
            SDL_Log("Lib/Circle.cpp/Draw/SDL_RenderDrawLine/cela na pas fonctionné : %s", SDL_GetError());
            return;
        }
        px1 = px2;
        py1 = py2;
    }
}