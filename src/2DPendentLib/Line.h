#pragma once
#include <SDL3/SDL.h>
#include "Geometry.h"
#include "Drawable.h"

class Line : public Geometry, public Drawable
{
private:

	Vector2f m_endPoint; // Point de fin de la ligne

public:

	Line(Vector2f p_startPoint, Vector2f p_endPoint, Color p_color);

	void SetEndPosition(Vector2f p_endPosition, Vector2f p_anchor = { 0.5f, 0.5f });
	Vector2f GetEndPosition() { return m_endPoint; }

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};