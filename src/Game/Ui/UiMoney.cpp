#include "pch.h"
#include "UiMoney.h"
#include "GameScene.h"
#include "SpriteContainer.h"



void UiMoney::OnInitialize()
{

	RemoveCollider("default");
	AddSprite("BackgroundUiMoney", "Bouton_ATM_Bank", GetDimension(), 0, {}, true);

	AddSprite("UiMoney", "UiMoney", { 32,32 }, 1, {}, true);
	GetSprite("UiMoney")->SetPosition(GetPosition({0.2,0.5f}));

	Vector2f spriteRightPos = GetSprite()->GetPosition({ 1,0.5f });
	Vector2f textPos = { spriteRightPos.GetX() - 20, spriteRightPos.GetY() };
	Debug::Get()->CreateText("Money", GetPosition(), std::to_string((int)m_currentValue), ColorsNamed::White, true);

}

void UiMoney::OnUpdate()
{
	
	Debug::Get()->GetText("Money")->SetText(std::to_string(m_gameData.GetPlayerData().m_money));
}

void UiMoney::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);
	GetSprite("UiMoney")->SetPosition(GetPosition({ 0.2,0.5f }));
	Vector2f spriteRightPos = GetSprite()->GetPosition({ 1,0.5f });
	Vector2f textPos = { spriteRightPos.GetX() - 20, spriteRightPos.GetY() };
	Debug::Get()->GetText("Money")->SetPosition(GetPosition());
}

void UiMoney::SetIsVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);
	GetSprite("UiMoney")->SetIsVisible(p_isVisible);
	Debug::Get()->GetText("Money")->SetIsVisible(p_isVisible);
}