#include "pch.h"
#include "UiLife.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiLife::OnInitialize()
{
	GameScene* gameScene = GetScene<GameScene>();
	m_currentValue = gameScene->GetPlayer()->GetStats().m_Hp;
	m_maxValue = gameScene->GetPlayer()->GetStats().m_maxHp;

	AddSprite("LifeFull", "UiLifeFull", GetDimension(), 0, {}, true);
	AddSprite("LifeEmpty", "UiEmpty", GetDimension(), 0, {}, true);

	GetSprite("LifeEmpty")->SetDimension(GetDimension());
	GetSprite("LifeFull")->SetDimension(GetDimension());
	
	
	ShowCollider(true);
	SyncSprites();
}

void UiLife::OnUpdate()
{
	GameScene* gameScene = GetScene<GameScene>();
	int hp = gameScene->GetPlayer()->GetStats().m_Hp;

	if (m_currentValue == hp)
		return;

	m_currentValue = hp;

	Vector2i dim = GetDimension();
	dim.GetY() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("LifeFull")->SetDimension(dim);
	GetSprite("LifeFull")->SetPosition(GetPosition({ 0,1 }), { 0,1 });
}


