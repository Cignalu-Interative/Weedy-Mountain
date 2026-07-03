#include "pch.h"
#include "UiMoney.h"
#include "GameScene.h"
#include "SpriteContainer.h"


void UiMoney::OnInitialize()
{
	GameScene* gameScene = GetScene<GameScene>();

	m_currentValue = gameScene->GetPlayer()->GetMoney();

	AddSprite("UiMoney", "UiMoney", { 32,32 }, 0, {}, true);
	GetSprite("UiMoney")->SetPosition(GetPosition({0.2,0.5f}));
	Vector2f textUiMoneyPos = {};
	GetScene()->CreateText("Money", textUiMoneyPos, std::to_string((int)m_currentValue),ColorsNamed::White, true);
	Vector2f spriteRightPos = GetSprite()->GetPosition({ 1,0.5f });
	GetScene()->GetGeometry<Text>("Money")->SetPosition(spriteRightPos);
	
	GetCollider()->SetIsTrigger(true);
}

void UiMoney::OnUpdate()
{
	GetScene()->GetGeometry<Text>("Money")->SetText("  " + std::to_string(GetScene<GameScene>()->GetPlayer()->GetMoney()));
}
