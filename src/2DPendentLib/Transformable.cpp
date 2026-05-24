#include "pch.h"
#include "Transformable.h"
#include "Utils.h"

Transformable::Transformable(Vector2f p_position, Vector2i p_dimension)
{
	m_dimension = p_dimension;
	m_position = p_position;
}

void Transformable::SetAngle(float p_angle)
{
	m_angle = Utils::NormalizeAngle(p_angle);
}

void Transformable::SetDimension(Vector2i p_dimension)
{
	m_dimension.SetX(p_dimension.GetX());
	m_dimension.SetY(p_dimension.GetY());
}

void Transformable::Resize(Vector2i p_newDimension)
{
	float factorWidth = p_newDimension.GetX() / (float)m_dimension.GetX();
	float factorHeight = p_newDimension.GetY() / (float)m_dimension.GetY();

	float factorMin = std::min(factorWidth, factorHeight);

	if (p_newDimension.GetX() == -1)
		factorMin = factorHeight;
	else
		if (p_newDimension.GetY() == -1)
			factorMin = factorWidth;

	m_dimension.SetX(static_cast<int>(m_dimension.GetX() * factorMin));
	m_dimension.SetY(static_cast<int>(m_dimension.GetY() * factorMin));

	//float actualWidth = static_cast<float>(m_dimension.GetX());
	//float actualHeight = static_cast<float>(m_dimension.GetY());
	//float width = static_cast<float>(p_newDimension.GetX());
	//float height = static_cast<float>(p_newDimension.GetY());
	//if (actualWidth <= 0 || actualHeight <= 0)
	//{
	//	return;
	//}

	//float factorWidth, factorHeight;

	//// Si width=0  on force factorWidth à l'infini (on ignore cette contrainte)
	//if (width == 0) {
	//	factorWidth = FLT_MAX;
	//	factorHeight = static_cast<float>(height) / actualHeight;
	//}
	//// Si height=0  on force factorHeight à l'infini
	//else if (height == 0) {
	//	factorWidth = static_cast<float>(width) / actualWidth;
	//	factorHeight = FLT_MAX;
	//}
	//// Les deux dimensions données  logique classique
	//else {
	//	factorWidth = static_cast<float>(width) / actualWidth;
	//	factorHeight = static_cast<float>(height) / actualHeight;
	//}

	//float factorMax = std::max(factorWidth, factorHeight);

	//// Arrondi propre
	//int newWidth = static_cast<int>(actualWidth * factorMax + 0.5f);
	//int newHeight = static_cast<int>(actualHeight * factorMax + 0.5f);

	//m_dimension.SetX(std::max(1, newWidth - 10));
	//m_dimension.SetY(std::max(1, newHeight));
}

void Transformable::SetPosition(const Vector2f& p_position, Vector2f p_anchor)
{
	if(m_dimension.GetX() <= 0 || m_dimension.GetY() <= 0)
	{
		m_position = p_position;
		return;
	}
	else
	{
	m_position.SetX(p_position.GetX() - m_dimension.GetX() * p_anchor.GetX());
	m_position.SetY(p_position.GetY() - m_dimension.GetY() * p_anchor.GetY());
	}
}

Vector2f Transformable::GetPosition(Vector2f p_anchor) const
{
	if (m_dimension.GetX() <= 0 || m_dimension.GetY() <= 0)
	{
		return m_position;
	}
	else
	{
		return Vector2f(m_position.GetX() + m_dimension.GetX() * p_anchor.GetX(), m_position.GetY() + m_dimension.GetY() * p_anchor.GetY());
	}
}

void Transformable::Move(Vector2f p_translation)
{
	m_position.SetX(m_position.GetX() + p_translation.GetX());
	m_position.SetY(m_position.GetY() + p_translation.GetY());
}