#include "pch.h"
#include "UiStats.h"

void UiStats::OnInitialize()
{
	Vector2i dim = {8 * 4,32 * 4};
	Vector2i dimension = GetDimension();
	
	Vector2f lifePos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f staminaPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY() };
	Vector2f toxicityPos = GetPosition();

	//int drawLayer = GetDrawLayer() + 1;

	m_uiLife = GetScene()->CreateEntity<UiLife>(lifePos,dim, "", Shape::Rectangle, true);
	//m_uiLife->SetDrawLayer(drawLayer);

	m_uiStamina = GetScene()->CreateEntity<UiStamina>(staminaPos,dim, "", Shape::Rectangle, true);
	//m_uiStamina->SetDrawLayer(drawLayer);

	m_uiToxicity = GetScene()->CreateEntity<UiToxicity>(toxicityPos,dim, "", Shape::Rectangle, true);
	//m_uiToxicity->SetDrawLayer(drawLayer);

	m_uiLife->GetCollider()->SetIsTrigger(true);
	m_uiStamina->GetCollider()->SetIsTrigger(true);
	m_uiToxicity->GetCollider()->SetIsTrigger(true);
	GetCollider()->SetIsTrigger(true);
}

void UiStats::OnUpdate()
{
}
