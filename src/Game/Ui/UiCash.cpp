#include "pch.h"
#include "UiCash.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiCash::OnInitialize()
{
	RemoveCollider("default");

	AddSprite("BackgroundUiCash", "Bouton_ATM_Cash", GetDimension(), 0, {}, true);

	AddSprite("UiCash", "UiCash", { 32,32 }, 1, {}, true);
	GetSprite("UiCash")->SetPosition(GetPosition({ 0.2,0.5f }));

	Vector2f spriteRightPos = GetSprite()->GetPosition({ 1,0.5f });
	Vector2f textPos = { spriteRightPos.GetX() - 20, spriteRightPos.GetY() };
	Debug::Get()->CreateText("UiCashText", textPos, std::to_string((int)m_currentValue), ColorsNamed::White, true);

}

void UiCash::OnUpdate()
{
	Debug::Get()->GetText("UiCashText")->SetText(std::to_string(m_gameData.GetPlayerData().m_cash));
}

void UiCash::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);
	GetSprite("UiCash")->SetPosition(GetPosition({ 0.2,0.5f }));
	Vector2f spriteRightPos = GetSprite()->GetPosition({ 1,0.5f });
	Vector2f textPos = { spriteRightPos.GetX() - 20, spriteRightPos.GetY() };
	Debug::Get()->GetText("UiCashText")->SetPosition(textPos);
}