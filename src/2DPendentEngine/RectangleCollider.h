#pragma once
#include "Collider.h"
#include "2DPendentLib/Face.h"
#include "2DPendentLib/Drawable.h"

class Rectangle;


class RectangleCollider : public Collider, public Drawable
{
private:

	Rectangle* m_rectangle; //Rectangle utilisé pour les calculs de collision

public :

    RectangleCollider() = default;
    ~RectangleCollider() = default;

    void Initialize(Vector2f p_position, Vector2i p_dimension);

	//bool IsColliding(Collider* p_other) override;

    Face FaceColliding(RectangleCollider& p_other);

    float OverlapInX(RectangleCollider& p_other);
    float OverlapInY(RectangleCollider& p_other);

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};