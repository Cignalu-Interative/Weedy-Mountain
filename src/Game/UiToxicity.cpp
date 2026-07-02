#include "pch.h"
#include "UiToxicity.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiToxicity::OnInitialize()
{
	GameScene* gameScene = GetScene<GameScene>();
	m_currentValue = gameScene->GetPlayer()->GetStats().m_maxToxicity;
	m_maxValue = gameScene->GetPlayer()->GetStats().m_maxToxicity;

	AddSprite("UiToxicity", "UiToxicityFull", GetDimension(), 0, {}, true);
	AddSprite("ToxicityEmpty", "UiEmpty", GetDimension(), 0, {}, true);

	GetSprite("ToxicityEmpty")->SetDimension(GetDimension());
	GetSprite("UiToxicity")->SetDimension(GetDimension());

	ShowCollider(true);
	SyncSprites();
}

void UiToxicity::OnUpdate()
{
	GameScene* gameScene = GetScene<GameScene>();
	int hp = gameScene->GetPlayer()->GetStats().m_toxicity;

	if (m_currentValue == hp)
		return;

	m_currentValue = hp;

	Vector2i dim = GetDimension();
	dim.GetY() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("UiToxicity")->SetDimension(dim);
	GetSprite("UiToxicity")->SetPosition(GetPosition({ 0,1 }), { 0,1 });
}
