#include "pch.h"
#include "UiJobs.h"
#include "Player.h"
#include "GameScene.h"
#include "SpriteContainer.h"

#include "UiFarmer.h"
#include "UiMiner.h"
#include "UiGatherer.h"

void UiJobs::OnInitialize()
{

	GetCollider()->SetIsTrigger(true);
	Vector2i dim = GetDimension() / 8;
	Vector2i dimension = GetDimension();

	Vector2f farmerPos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f gathererPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY() };
	Vector2f minerPos = GetPosition();
	Vector2f textPos = GetPosition({ 0.5f, 0 }) + Vector2f(0, 60);

	m_gathererUi = GetScene()->CreateEntity<UiGatherer>(gathererPos, dim, "", Shape::Rectangle, true);
	m_farmerUi = GetScene()->CreateEntity<UiFarmer>(farmerPos, dim, "", Shape::Rectangle, true);
	m_minerUi = GetScene()->CreateEntity<UiMiner>(minerPos, dim, "", Shape::Rectangle, true);

	AddSprite("background", "InventoryWeedyMountain", GetDimension(), 0, {},true);
	GetSprite("background")->SetDimension(GetDimension());
	GetSprite("background")->SetPosition(GetPosition());
	SetDrawLayer(GetDrawLayer() - 1);

	Debug::Get()->CreateText("metier", textPos, "JOBS", ColorsNamed::White, true);
	Debug::Get()->GetText("metier")->SetFontSize(50);

}

void UiJobs::SetIsVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);

	m_gathererUi->SetIsVisible(p_isVisible);
	m_farmerUi->SetIsVisible(p_isVisible);
	m_minerUi->SetIsVisible(p_isVisible);

	Debug::Get()->GetText("metier")->SetIsVisible(p_isVisible);
	Debug::Get()->GetText("FarmerLvL")->SetIsVisible(p_isVisible);
	Debug::Get()->GetText("MinerLvL")->SetIsVisible(p_isVisible);
	Debug::Get()->GetText("GathererLvL")->SetIsVisible(p_isVisible);

}
