#include "pch.h"
#include "Geometry.h"

Geometry::Geometry(Vector2f p_position, Vector2i p_dimension, Color p_color) : Transformable(p_position, p_dimension)
{
	m_color = p_color;
}