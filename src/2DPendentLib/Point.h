#pragma once
#include <SDL3/SDL.h>
#include "Geometry.h"
#include "Drawable.h"

class Point : public Geometry , public Drawable
{
private:

	bool m_clampToScreen = false; // Si true, la geometry sera dessiné à la position la plus proche sur l'écran si sa position dépasse les limites de l'écran

public:

	Point(Vector2f p_position, Color p_color);
	~Point() = default;

	void SetIsClamp(bool p_isClamp) { m_clampToScreen = p_isClamp; }
	bool GetIsClamp() { return m_clampToScreen; }

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

};

