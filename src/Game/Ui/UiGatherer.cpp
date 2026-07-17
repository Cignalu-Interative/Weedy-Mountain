#include "pch.h"
#include "UiGatherer.h"
#include "Player.h"
#include "GameScene.h"
#include "SpriteContainer.h"


void UiGatherer::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);

	AddSprite("UiGathererFull", "fullHorizontalWhite", {120,64}, 0, {}, true);
	AddSprite("UiGathererEmpty", "emptyHorizontal", { 120,64 }, 0, {}, true);

	AddSprite("GathererAsset", "cisailles", { 120,64 }, 0, {}, true);

	Vector2f barPos = GetSprite("GathererAsset")->GetPosition() + Vector2f(0, 50);
	GetSprite("UiGathererEmpty")->SetPosition(barPos);

	Vector2f textPos = GetSprite("GathererAsset")->GetPosition() + Vector2f(0,80);
	Debug::Get()->CreateText("GathererLvL", textPos, std::to_string(gm.GetPlayerData().GetJobByName(JobType::Gatherer)->m_lvl), Color(255, 255, 255, 255), true);

}

void UiGatherer::OnUpdate()
{
	Debug::Get()->GetText("GathererLvL")->SetText(std::to_string(gm.GetPlayerData().GetJobByName(JobType::Gatherer)->m_lvl));

	m_currentValue =gm.GetPlayerData().GetJobByName(JobType::Gatherer)->m_exp;
	m_maxValue = gm.GetPlayerData().GetJobByName(JobType::Gatherer)->m_expNeeded;

	Vector2i dim = GetSprite("UiGathererEmpty")->GetDimension();
	dim.GetX() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("UiGathererFull")->SetDimension(dim);
	GetSprite("UiGathererFull")->SetPosition(GetSprite("UiGathererEmpty")->GetPosition({0.01f,0.5f}), { 0,0.5f });
}
