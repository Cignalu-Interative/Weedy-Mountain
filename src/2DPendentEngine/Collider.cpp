#include "pch.h"
#include "Collider.h"
#include "2DPendentLib/Vector2.h"
//
//void Collider::Initialize(Shape p_shape, Vector2f p_position, Vector2i p_dimension, Color p_color)
//{
//	SetShape(p_shape);
//	if (m_shape == Shape::Circle)
//	{
//		m_circle.Initialize(p_position, p_dimension.GetX() / 2, p_color);
//	}
//	else if (m_shape == Shape::Rectangle)
//	{
//		m_rectangle.Initialize(p_position, p_dimension, p_color);
//	}
//}
//
//void Collider::SetShape(Shape p_type)
//{
//	if (p_type == Shape::Circle)
//	{
//		m_shape = Shape::Circle;
//	}
//	else if (p_type == Shape::Rectangle)
//	{
//		m_shape = Shape::Rectangle;
//	}
//}
//
//void Collider::SetPosition(Vector2f p_position, Vector2f p_anchor)
//{
//	if (m_shape == Shape::Circle)
//	{
//		m_circle.SetPosition(p_position, p_anchor);
//	}
//	else if (m_shape == Shape::Rectangle)
//	{
//		m_rectangle.SetPosition(p_position, p_anchor);
//	}
//}
//
//Vector2f Collider::GetPosition(Vector2f p_position, Vector2f p_anchor)
//{
//	if (m_shape == Shape::Circle)
//	{
//		return m_circle.GetPosition(p_anchor);
//	}
//	else if (m_shape == Shape::Rectangle)
//	{
//		return m_rectangle.GetPosition(p_anchor);
//	}
//}
