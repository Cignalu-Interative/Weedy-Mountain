#include "pch.h"
#include "UiMiner.h"
#include "Player.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiMiner::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);

	AddSprite("UiMinerFull", "fullHorizontalRed", { 120,64 }, 0, {}, true);
	AddSprite("UiMinerEmpty", "emptyHorizontal", { 120,64 }, 0, {}, true);

	AddSprite("MinerAsset", "Pickaxe", { 120,64 }, 0, {}, true);

	Vector2f barPos = GetSprite("MinerAsset")->GetPosition() + Vector2f(0, 50);
	GetSprite("UiMinerEmpty")->SetPosition(barPos);

	Vector2f textPos = GetSprite("MinerAsset")->GetPosition() + Vector2f(0, 80);
	Debug::Get()->CreateText("MinerLvL", textPos, std::to_string(gm.GetPlayerData().GetJobByName(JobType::Miner)->m_lvl), Color(255, 255, 255, 255), true);

}

void UiMiner::OnUpdate()
{
	Debug::Get()->GetText("MinerLvL")->SetText(std::to_string(gm.GetPlayerData().GetJobByName(JobType::Miner)->m_lvl));

	m_currentValue = gm.GetPlayerData().GetJobByName(JobType::Miner)->m_exp;
	m_maxValue = gm.GetPlayerData().GetJobByName(JobType::Miner)->m_expNeeded;

	Vector2i dim = GetSprite("UiMinerEmpty")->GetDimension();
	dim.GetX() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("UiMinerFull")->SetDimension(dim);
	GetSprite("UiMinerFull")->SetPosition(GetSprite("UiMinerEmpty")->GetPosition({ 0.01f,0.5f }), { 0,0.5f });

}

void UiMiner::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);

	Vector2f barPos = GetSprite("MinerAsset")->GetPosition() + Vector2f(0, 50);
	GetSprite("UiMinerEmpty")->SetPosition(barPos);

	Vector2f textPos = GetSprite("MinerAsset")->GetPosition() + Vector2f(0, 80);
	Debug::Get()->GetText("MinerLvL")->SetPosition(textPos);
}
