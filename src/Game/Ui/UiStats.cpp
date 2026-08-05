#include "pch.h"
#include "UiStats.h"

void UiStats::OnInitialize()
{
	RemoveCollider("default");
	Vector2i dim = {24,128};
	Vector2i dimension = GetDimension();
	
	Vector2f lifePos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f staminaPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY() };
	Vector2f toxicityPos = GetPosition();

	int drawLayer = GetDrawLayer() - 30;

	m_uiLife = GetScene()->CreateEntity<UiLife>(lifePos,dim, "", Shape::Rectangle, true);
	m_uiLife->SetDrawLayer(drawLayer);
	m_uiLife->RemoveCollider("default");

	m_uiStamina = GetScene()->CreateEntity<UiStamina>(staminaPos,dim, "", Shape::Rectangle, true);
	m_uiStamina->SetDrawLayer(drawLayer);
	m_uiStamina->RemoveCollider("default");

	m_uiToxicity = GetScene()->CreateEntity<UiToxicity>(toxicityPos,dim, "", Shape::Rectangle, true);
	m_uiToxicity->SetDrawLayer(drawLayer);
	m_uiToxicity->RemoveCollider("default");
}

void UiStats::OnUpdate()
{
}

void UiStats::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);

	Vector2i dimension = GetDimension();
	Vector2f lifePos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f staminaPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY() };
	Vector2f toxicityPos = GetPosition();

	m_uiLife->SetPosition(lifePos);
	m_uiStamina->SetPosition(staminaPos);
	m_uiToxicity->SetPosition(toxicityPos);
}
