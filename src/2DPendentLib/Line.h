#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include "Geometry.h"
#include "Drawable.h"

class Line : public Geometry, public Drawable
{
private:

	Vector2f m_endPoint; // Point de fin de la ligne
	bool m_clampToScreen = false; // Si true, la geometry sera dessiné à la position la plus proche sur l'écran si sa position dépasse les limites de l'écran

public:

	Line(Vector2f p_startPoint, Vector2f p_endPoint, Color p_color);

	void SetIsClamp(bool p_isClamp) { m_clampToScreen = p_isClamp; }
	bool GetIsClamp() { return m_clampToScreen; }

	void SetEndPosition(Vector2f p_endPosition, Vector2f p_anchor = { 0.5f, 0.5f });
	Vector2f GetEndPosition() { return m_endPoint; }

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};