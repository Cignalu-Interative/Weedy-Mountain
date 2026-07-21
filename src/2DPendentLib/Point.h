#pragma once
#include <SDL3/SDL.h>
#include "Geometry.h"
#include "Drawable.h"

class Point : public Geometry , public Drawable
{
private:

public:

	Point(Vector2f p_position, Color p_color);
	~Point() = default;

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

};

