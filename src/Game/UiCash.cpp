#include "pch.h"
#include "UiCash.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiCash::OnInitialize()
{
	AddSprite("UiCash", "UiCash", { 32,32 }, 0, {}, true);
	GetSprite("UiCash")->SetPosition(GetPosition({ 0.2,0.5f }));

	Vector2f spriteRightPos = GetSprite()->GetPosition({ 1,0.5f });
	Vector2f textPos = { spriteRightPos.GetX() + 20, spriteRightPos.GetY() };
	GetScene()->CreateText("UiCash", textPos, std::to_string((int)m_currentValue), ColorsNamed::White, true);

	GetCollider()->SetIsTrigger(true);
}

void UiCash::OnUpdate()
{
	GetScene()->GetGeometry<Text>("UiCash")->SetText(std::to_string(GetScene<GameScene>()->GetPlayer()->GetCash()));
}
