#include "pch.h"
#include "UiToxicity.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiToxicity::OnInitialize()
{
	m_currentValue = GameData::Get().GetPlayerData().m_stats.m_maxToxicity;
	m_maxValue = GameData::Get().GetPlayerData().m_stats.m_maxToxicity;

	AddSprite("Drugs", "Drugs", { 32,32 }, 0, {}, true);
	AddSprite("UiToxicity", "UiToxicityFull", GetDimension(), 0, {}, true);
	AddSprite("ToxicityEmpty", "UiEmpty", GetDimension(), 0, {}, true);

	GetSprite("ToxicityEmpty")->SetDimension(GetDimension());
	GetSprite("UiToxicity")->SetDimension(GetDimension());

	SyncSprites();

	GetSprite("Drugs")->SetPosition(GetSprite("ToxicityEmpty")->GetPosition({ 0.5f,0 }),{0.5f,1});
}

void UiToxicity::OnUpdate()
{
	int toxicity = GameData::Get().GetPlayerData().m_stats.m_toxicity;

	/*if (toxicity == 0)
		SetIsVisible(false);
	else
		SetIsVisible(true);*/

	if (m_currentValue == toxicity)
		return;

	m_currentValue = toxicity;

	Vector2i dim = GetDimension();
	dim.GetY() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("UiToxicity")->SetDimension(dim);
	GetSprite("UiToxicity")->SetPosition(GetPosition({ 0,1 }), { 0,1 });
}

void UiToxicity::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);
	GetSprite("Drugs")->SetPosition(GetSprite("ToxicityEmpty")->GetPosition({ 0.5f,0 }), { 0.5f,1 });
}
