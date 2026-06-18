#include "pch.h"
#include "CircleCollider.h"
#include "Collision.h"
#include "2DPendentLib/Render.h"
#include "2DPendentLib/Camera.h"
#include "Entity.h"

void CircleCollider::Initialize(Vector2f p_position, Vector2i p_dimension)
{
    m_position = p_position;
    m_dimension = p_dimension;
    m_shape = Shape::Circle;

    SetPosition(p_position);
}

void CircleCollider::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
    if (IsShow() == false)
    {
        return;
    }

    Vector2f pos = GetPosition();
    Vector2i dim = GetDimension();
    Color col = GetColor();

    if (GetOwner()->GetIsGUI())
    {
        dim /= p_camera->GetZoom();
    }

    if (m_circle == nullptr)
    {
		m_circle = new Circle();
		m_circle->Initialize(pos, dim, col);
    }
	m_circle->SetDimension(dim);
	m_circle->SetPosition(pos);
	m_circle->SetColor(col);
	m_circle->Draw(f_render, f_window, p_camera);
}
    


//bool CircleCollider::IsColliding(Collider* p_other)
//{
//    if (p_other->GetShape() == Shape::Circle)
//    {
//        Vector2f distance = GetPosition() - p_other->GetPosition();
//
//        float sqrLength = (distance.GetX() * distance.GetX()) + (distance.GetY() * distance.GetY());
//
//        float circleRadius = GetDimension().GetX();
//        float otherCircleRadius = p_other->GetDimension().GetX();
//
//        float sqrRadius = (circleRadius + otherCircleRadius) * (circleRadius + otherCircleRadius);
//
//        return sqrLength < sqrRadius;
//    }
//    else if (p_other->GetShape() == Shape::Rectangle)
//    {
//		Vector2f circlePosition = GetPosition();
//		float circleRadius = GetDimension().GetX();
//
//		Vector2f rectPosition = p_other->GetPosition({0,0});
//		Vector2i rectDimension = p_other->GetDimension();
//
//		float testX = circlePosition.GetX();
//		float testY = circlePosition.GetY();
//
//        if(circlePosition.GetX() < rectPosition.GetX()) //test left edge
//            testX = rectPosition.GetX();
//		else if (circlePosition.GetX() > rectPosition.GetX() + rectDimension.GetX()) // right edge
//            testX = rectPosition.GetX() + rectDimension.GetX();
//
//        if(circlePosition.GetY() < rectPosition.GetY()) // top edge
//            testY = rectPosition.GetY();
//        else if (circlePosition.GetY() > rectPosition.GetY() + rectDimension.GetY())// bottom edge
//            testY = rectPosition.GetY() + rectDimension.GetY();
//
//		Vector2f distance = {circlePosition.GetX() - testX, circlePosition.GetY() - testY};
//
//		float sqrtLength = (distance.GetX() * distance.GetX()) + (distance.GetY() * distance.GetY());
//
//		return sqrtLength <= (circleRadius * circleRadius);
//    }
//}