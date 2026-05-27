#include "Camera.h"

template<typename T>
Vector2<T> Camera::WorldToScreen(Vector2<T> p_worldPos) const
{
	Vector2<T> screenPos;
	screenPos.SetX((p_worldPos.GetX() - GetPosition().GetX()) * m_zoomFactor + GetDimension().GetX() / 2);
	screenPos.SetY((p_worldPos.GetY() - GetPosition().GetY()) * m_zoomFactor + GetDimension().GetY() / 2);
	return screenPos;
}

template<typename T>
Vector2<T> Camera::ScreenToWorld(Vector2<T> p_screenPos) const
{
	Vector2<T> worldPos;

	worldPos.SetX((p_screenPos.GetX() - GetDimension().GetX() / 2.f) / m_zoomFactor + GetPosition().GetX());
	worldPos.SetY((p_screenPos.GetY() - GetDimension().GetY() / 2.f) / m_zoomFactor + GetPosition().GetY());
	return worldPos;
}
