#include "pch.h"
#include "UiMoney.h"
#include "GameScene.h"
#include "SpriteContainer.h"



void UiMoney::OnInitialize()
{
	AddSprite("UiMoney", "UiMoney", { 32,32 }, 0, {}, true);
	GetSprite("UiMoney")->SetPosition(GetPosition({0.2,0.5f}));

	Vector2f spriteRightPos = GetSprite()->GetPosition({ 1,0.5f });
	Vector2f textPos = { spriteRightPos.GetX() + 20, spriteRightPos.GetY() };
	Debug::Get()->CreateText("Money", textPos, std::to_string((int)m_currentValue), ColorsNamed::White, true);
	
	GetCollider()->SetIsTrigger(true);
}

void UiMoney::OnUpdate()
{
	
	Debug::Get()->GetText("Money")->SetText(std::to_string(m_gameData.GetPlayerData().m_money));
}
