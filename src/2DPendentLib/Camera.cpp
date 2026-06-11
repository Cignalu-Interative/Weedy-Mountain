#include "pch.h"
#include "Camera.h"
#include "2DPendentLib/Utils.h"
#include "Render.h"
#include "Window.h"
#include "2DPendentEngine/Entity.h"
#include <SDL.h>

//Camera::Camera(Vector2f p_position, Vector2i p_dimension)
//{
//    SetDimension(p_dimension);
//    SetPosition(p_position);
//}

Camera::Camera(Vector2i p_dimension, Vector2f p_position, Vector2f p_anchor, float p_zoomFactor)
{
    SetDimension(p_dimension);
    SetPosition(p_position, p_anchor);
    m_zoomFactor = p_zoomFactor;
}

void Camera::SetZoom(float p_zoomFactor)
{
    m_zoomFactor = Utils::Clamp(p_zoomFactor, m_minZoom, m_maxZoom);
}

void Camera::ZoomIn(float p_amount)
{
	float zoom =m_zoomFactor + Utils::Clamp(p_amount, m_minZoom, m_maxZoom);
	SetZoom(zoom);
}

void Camera::ZoomOut(float p_amount)
{
	float zoom = m_zoomFactor - Utils::Clamp(p_amount, m_minZoom, m_maxZoom);
	SetZoom(zoom);
}

void Camera::Follow(Entity* p_entity, float p_deltaTime)
{
    float test = exp(-m_smoothSpeed * p_deltaTime);
    float factor = 1 - exp(-m_smoothSpeed * p_deltaTime);

    float lerpX = std::lerp(GetPosition().GetX(), p_entity->GetPosition().GetX(), factor);
    float lerpY = std::lerp(GetPosition().GetY(), p_entity->GetPosition().GetY(), factor);

    SetPosition({ lerpX,lerpY });
}

void Camera::ClampToWorld()
{
}

void Camera::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
    Vector2i margin = { 2, 2 };

    // Dimension du frustum en monde = dimension écran / zoom
    Vector2i worldDimension(
        Vector2f(
            (GetDimension().GetX() - margin.GetX()) / m_zoomFactor,
            (GetDimension().GetY() - margin.GetY()) / m_zoomFactor
        )
    );

    auto corners = Utils::GetCorners(GetPosition(), worldDimension, GetAngle(), GetRotationPoint());

    SDL_SetRenderDrawColor(f_render->m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    for (int i = 0; i < 4; i++)
    {
        Vector2f a = p_camera->WorldToScreen(corners[i]);
        Vector2f b = p_camera->WorldToScreen(corners[(i + 1) % 4]);
        SDL_RenderDrawLineF(f_render->m_renderer, a.GetX(), a.GetY(), b.GetX(), b.GetY());
    }
}