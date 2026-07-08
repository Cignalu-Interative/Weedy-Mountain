#include "pch.h"
#include "UiLife.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiLife::OnInitialize()
{
	Player* player = dynamic_cast<Player*>(GetScene()->GetEntityByTag(1));
	if (player == nullptr)
		return;

	m_currentValue = player->GetStats().m_hp;
	m_maxValue = player->GetStats().m_maxHp;

	AddSprite("Heart", "Heart", { 32,32 }, 0, {}, true);
	AddSprite("LifeFull", "UiLifeFull", GetDimension(), 0, {}, true);
	AddSprite("LifeEmpty", "UiEmpty", GetDimension(), 0, {}, true);

	GetSprite("LifeEmpty")->SetDimension(GetDimension());
	GetSprite("LifeFull")->SetDimension(GetDimension());

	SyncSprites();

	GetSprite("Heart")->SetPosition(GetSprite("LifeEmpty")->GetPosition({ 0.5f,0 }), { 0.5f,1 });
}
	
void UiLife::OnUpdate()
{
	Player* player = dynamic_cast<Player*>(GetScene()->GetEntityByTag(1));
	if (player == nullptr)
		return;

	int hp = player->GetStats().m_hp;

	if (hp == player->GetStats().m_maxHp)
		SetIsVisible(false);
	else
		SetIsVisible(true);

	if (m_currentValue == hp)
		return;

	m_currentValue = hp;

	Vector2i dim = GetDimension();
	dim.GetY() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("LifeFull")->SetDimension(dim);
	GetSprite("LifeFull")->SetPosition(GetPosition({ 0,1 }), { 0,1 });
}


