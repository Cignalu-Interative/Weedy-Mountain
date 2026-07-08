#include "pch.h"
#include "UiFarmer.h"
#include "Player.h"
#include "GameScene.h"
#include "SpriteContainer.h"
void UiFarmer::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);
	m_player = GetScene<GameScene>()->GetPlayer();

	AddSprite("UiFarmerFull", "fullHorizontalBlue", { 120,64 }, 0, {}, true);
	AddSprite("UiFarmerEmpty", "emptyHorizontal", { 120,64 }, 0, {}, true);

	AddSprite("FarmerAsset", "faucille", { 120,64 }, 0, {}, true);

	Vector2f barPos = GetSprite("FarmerAsset")->GetPosition() + Vector2f(0, 50);
	GetSprite("UiFarmerEmpty")->SetPosition(barPos);

	Vector2f textPos = GetSprite("FarmerAsset")->GetPosition() + Vector2f(0, 80);
	Debug::Get()->CreateText("FarmerLvL", textPos, std::to_string(m_player->GetJobByName(JobType::Farmer).m_lvl), Color(255, 255, 255, 255), true);

}

void UiFarmer::OnUpdate()
{
	Debug::Get()->GetText("FarmerLvL")->SetText(std::to_string(m_player->GetJobByName(JobType::Farmer).m_lvl));

	m_currentValue = m_player->GetJobByName(JobType::Farmer).m_exp;
	m_maxValue = m_player->GetJobByName(JobType::Farmer).m_expNeeded;

	Vector2i dim = GetSprite("UiFarmerEmpty")->GetDimension();
	dim.GetX() *= m_currentValue / m_maxValue; // 0.f

	GetSprite("UiFarmerFull")->SetDimension(dim);
	GetSprite("UiFarmerFull")->SetPosition(GetSprite("UiFarmerEmpty")->GetPosition({ 0.01f,0.5f }), { 0,0.5f });


}