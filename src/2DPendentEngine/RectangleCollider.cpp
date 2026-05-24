#include "pch.h"
#include "RectangleCollider.h"
#include "2DPendentLib/Render.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentLib/Utils.h"

void RectangleCollider::Initialize(Vector2f p_position, Vector2i p_dimension)
{
    m_position = p_position;
    m_dimension = p_dimension;
    m_shape = Shape::Rectangle;

    SetPosition(p_position);
}

//bool RectangleCollider::IsColliding(Collider* p_other)
//{
//    if (p_other->GetShape() == Shape::Rectangle)// si collider c un rectangle
//    {
//        RectangleCollider* rectangle = (RectangleCollider*)p_other;
//        Vector2f otherRectPosition = rectangle->GetPosition();
//        Vector2i otherRectDimension = rectangle->GetDimension();
//
//        Vector2f position = GetPosition();
//        Vector2i dimension = GetDimension();
//
//        return !(
//            otherRectPosition.GetX() > position.GetX() + dimension.GetX() ||       
//            otherRectPosition.GetX() + otherRectDimension.GetX() < position.GetX() ||    
//            otherRectPosition.GetY() > position.GetY() + dimension.GetY() ||       
//            otherRectPosition.GetY() + otherRectDimension.GetY() < position.GetY()           
//            );
//    }
//    else if (p_other->GetShape() == Shape::Circle) // si collider c un cercle
//    {
//        return p_other->IsColliding(this);
//	}
//}

Face RectangleCollider::FaceColliding(RectangleCollider& p_other)
{
   /* if (IsColliding(&p_other) == false)
        return -1;*/
    Vector2f position = GetPosition();
    Vector2i dimension = GetDimension();
    Vector2f otherPosition = p_other.GetPosition();
    Vector2i otherDimension = p_other.GetDimension();


    float overlapY = OverlapInY(p_other);
    float overlapX = OverlapInX(p_other);

    // Verticale (Haut / Bas)
    if (overlapY < overlapX)
    {
        // Haut
        if (position.GetY() > otherPosition.GetY())
            return Face::UP;
        else
            return Face::DOWN;
    }

    // Horizontale (Gauche / Droite)
    if (overlapY > overlapX)
    {
        // Gauche
        if (position.GetX() > otherPosition.GetX())
            return Face::LEFT;
        else
            return Face::RIGHT;
    }
}

float RectangleCollider::OverlapInX(RectangleCollider& p_other)
{
    Vector2f position = GetPosition();
    Vector2i dimension = GetDimension();
    Vector2f otherPosition = p_other.GetPosition();
    Vector2i otherDimension = p_other.GetDimension();

    float overlapX = (otherDimension.GetX() / 2.0f + dimension.GetX() / 2.0f) -
        std::abs((otherPosition.GetX() + otherDimension.GetX() / 2.0f) -
            (position.GetX() + dimension.GetX() / 2.0f));
    return overlapX;
}

float RectangleCollider::OverlapInY(RectangleCollider& p_other)
{
    Vector2f position = GetPosition();
    Vector2i dimension = GetDimension();
    Vector2f otherPosition = p_other.GetPosition();
    Vector2i otherDimension = p_other.GetDimension();

    float overlapY = (otherDimension.GetY() / 2.0f + dimension.GetY() / 2.0f) -
        std::abs((otherPosition.GetY() + otherDimension.GetY() / 2.0f) -
            (position.GetY() + dimension.GetY() / 2.0f));
    return overlapY;
}

void RectangleCollider::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
    if (IsShow() == false)
    {
        return;
    }

    if (m_rectangle == nullptr)
    {
        m_rectangle = new Rectangle();
        m_rectangle->Initialize(GetPosition(), GetDimension(), GetColor());
    }
    m_rectangle->SetDimension(GetDimension());
    m_rectangle->SetPosition(GetPosition());
	m_rectangle->SetAngle(GetAngle());
    m_rectangle->SetColor(GetColor());
    m_rectangle->Draw(f_render, f_window, p_camera);
}