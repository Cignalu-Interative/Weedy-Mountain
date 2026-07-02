#include "pch.h"
#include "UiStats.h"

void UiStats::OnInitialize()
{
	Vector2i dim = { 8 * 5,32 * 5 };
	Vector2i dimension = GetDimension();
	
	Vector2f lifePos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f staminaPos = GetPosition();
	Vector2f toxicityPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY()};

	m_uiLife = GetScene()->CreateEntity<UiLife>(lifePos,dim, "", Shape::Rectangle, true);
	m_uiStamina = GetScene()->CreateEntity<UiStamina>(staminaPos,dim, "", Shape::Rectangle, true);
	m_uiToxicity = GetScene()->CreateEntity<UiToxicity>(toxicityPos,dim, "", Shape::Rectangle, true);

}

void UiStats::OnUpdate()
{
}
