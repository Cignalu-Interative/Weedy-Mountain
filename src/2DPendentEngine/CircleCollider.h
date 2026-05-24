#pragma once
#include "Collider.h"
#include "2DPendentLib/Drawable.h"
#include "2DPendentLib/Circle.h"

class CircleCollider : public Collider, public Drawable
{
private:

	Circle* m_circle;

public :

	CircleCollider() = default;
	~CircleCollider() = default;

	void Initialize(Vector2f p_position, Vector2i p_dimension);

	//bool IsColliding(Collider* p_other) override;

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

};

