#include "pch.h"
#include "UiStamina.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiStamina::OnInitialize()
{
	GameScene* gameScene = GetScene<GameScene>();
	m_currentValue = gameScene->GetPlayer()->GetStats().m_stamina;
	m_maxValue = gameScene->GetPlayer()->GetStats().m_maxStamina;

	AddSprite("Energy", "Energy", { 32,32 }, 0, {}, true);
	AddSprite("StaminaFull", "UiStaminaFull", GetDimension(), 0, {}, true);
	AddSprite("StaminaEmpty", "UiEmpty", GetDimension(), 0, {}, true);

	GetSprite("StaminaEmpty")->SetDimension(GetDimension());
	GetSprite("StaminaFull")->SetDimension(GetDimension());

	SyncSprites();
	
	GetSprite("Energy")->SetPosition(GetSprite("StaminaEmpty")->GetPosition({ 0.5f,0 }), { 0.5f,1 });
}

void UiStamina::OnUpdate()
{
	GameScene* gameScene = GetScene<GameScene>();
	int hp = gameScene->GetPlayer()->GetStats().m_stamina;

	if (m_currentValue == hp)
		return;

	m_currentValue = hp;

	Vector2i dim = GetDimension();
	dim.GetY() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("StaminaFull")->SetDimension(dim);
	GetSprite("StaminaFull")->SetPosition(GetPosition({ 0,1 }), { 0,1 });
}