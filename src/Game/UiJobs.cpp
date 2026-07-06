#include "pch.h"
#include "UiJobs.h"
#include "Player.h"
#include "GameScene.h"
#include "SpriteContainer.h"

void UiJobs::OnInitialize()
{
	m_player = GetScene<GameScene>()->GetPlayer();
	GetCollider()->SetIsTrigger(true);

	
	Vector2i dim = GetDimension() / 8;

	AddSprite("UiFarmerFull", "fullHorizontalBlue", dim, 0, {}, true);
	AddSprite("UiGathererFull", "fullHorizontalWhite", dim, 0, {}, true);
	AddSprite("UiMinerFull", "fullHorizontalRed", dim, 0, {}, true);

	AddSprite("UiFarmerEmpty", "emptyHorizontal", dim, 0, {}, true);
	AddSprite("UiGathererEmpty", "emptyHorizontal", dim, 0, {}, true);
	AddSprite("UiMinerEmpty", "emptyHorizontal", dim, 0, {}, true);

	AddSprite("FarmerAsset", "faucille", dim, 0, {}, true);
	AddSprite("MinerAsset", "cisailles", dim, 0, {}, true);
	AddSprite("GathererAsset", "Pickaxe", dim, 0, {}, true);

	AddSprite("background", "InventoryWeedyMountain", GetDimension(), 0, {}, true);

	Vector2i dimension = GetDimension();

	Vector2f farmerPos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f gathererPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY() };
	Vector2f minerPos = GetPosition();

	Vector2f offset = { 0,25 };
	Vector2f offsetLvL = { 0,60 };

	GetSprite("FarmerAsset")->SetPosition(farmerPos, { 0.5f,1 });
	GetSprite("MinerAsset")->SetPosition(minerPos, { 0.5f,1 });
	GetSprite("GathererAsset")->SetPosition(gathererPos, { 0.5f,1 });

	GetSprite("UiFarmerEmpty")->SetPosition(farmerPos + offset);
	GetSprite("UiGathererEmpty")->SetPosition(gathererPos + offset);
	GetSprite("UiMinerEmpty")->SetPosition(minerPos + offset);

	GetSprite("UiFarmerFull")->SetPosition(farmerPos + offset);
	GetSprite("UiGathererFull")->SetPosition(gathererPos + offset);
	GetSprite("UiMinerFull")->SetPosition(minerPos + offset);

	Debug::Get()->CreateText("farmerLvL", farmerPos + offsetLvL, std::to_string(m_player->GetJobByName(JobType::Farmer)->m_lvl), Color(255, 255, 255, 255), true);
	Debug::Get()->CreateText("gathererLvL", gathererPos + offsetLvL, std::to_string(m_player->GetJobByName(JobType::Gatherer)->m_lvl), Color(255, 255, 255, 255), true);
	Debug::Get()->CreateText("minerLvL", minerPos + offsetLvL, std::to_string(m_player->GetJobByName(JobType::Miner)->m_lvl), Color(255, 255, 255, 255), true);
}

void UiJobs::OnUpdate()
{
	Debug::Get()->GetText("farmerLvL")->SetText(std::to_string(m_player->GetJobByName(JobType::Farmer)->m_lvl));
	Debug::Get()->GetText("gathererLvL")->SetText(std::to_string(m_player->GetJobByName(JobType::Gatherer)->m_lvl));
	Debug::Get()->GetText("minerLvL")->SetText(std::to_string(m_player->GetJobByName(JobType::Miner)->m_lvl));


	int lvl;




}
