#include "pch.h"
#include "UiJobs.h"
#include "Player.h"
#include "GameScene.h"
#include "SpriteContainer.h"

#include "UiFarmer.h"
#include "UiMiner.h"
#include "UiGatherer.h"
#include "HUDScene.h"

void UiJobs::OnInitialize()
{
	RemoveCollider("default");
	Vector2i dim = GetDimension() / 8;
	Vector2i dimension = GetDimension();

	Vector2f farmerPos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f gathererPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY() };
	Vector2f minerPos = GetPosition();
	Vector2f textPos = GetPosition({ 0.5f, 0 }) + Vector2f(0, 60);

	m_gathererUi = GetScene()->CreateEntity<UiGatherer>(gathererPos, dim, "", Shape::Rectangle, true);
	m_farmerUi = GetScene()->CreateEntity<UiFarmer>(farmerPos, dim, "", Shape::Rectangle, true);
	m_minerUi = GetScene()->CreateEntity<UiMiner>(minerPos, dim, "", Shape::Rectangle, true);

	m_gathererUi->RemoveCollider("default");
	m_farmerUi->RemoveCollider("default");
	m_minerUi->RemoveCollider("default");

	AddSprite("background", "InventoryWeedyMountain", GetDimension(), 0, {},true);
	GetSprite("background")->SetDimension(GetDimension());
	GetSprite("background")->SetPosition(GetPosition());
	SetDrawLayer(GetDrawLayer() - 1);

	Debug::Get()->CreateText("metier", textPos, "JOBS", ColorsNamed::White, true);
	Debug::Get()->GetText("metier")->SetFontSize(50);

}

void UiJobs::Toggle()
{
	bool visible = GetIsVisible();
	Entity::SetIsVisible(!visible);

	m_gathererUi->SetIsVisible(!visible);
	m_farmerUi->SetIsVisible(!visible);
	m_minerUi->SetIsVisible(!visible);

	Debug::Get()->GetText("metier")->SetIsVisible(!visible);
	Debug::Get()->GetText("FarmerLvL")->SetIsVisible(!visible);
	Debug::Get()->GetText("MinerLvL")->SetIsVisible(!visible);
	Debug::Get()->GetText("GathererLvL")->SetIsVisible(!visible);

	GameData::Get().GetPlayerData().m_playerInventory->GetHotbar()->SetIsVisible(visible);

}
void UiJobs::Open()
{
	SetPosition((Vector2f)Application::Get()->GetWindow()->GetHalfWindowSize());

	Entity::SetIsVisible(true);

	m_gathererUi->SetIsVisible(true);
	m_farmerUi->SetIsVisible(true);
	m_minerUi->SetIsVisible(true);

	Debug::Get()->GetText("metier")->SetIsVisible(true);
	Debug::Get()->GetText("FarmerLvL")->SetIsVisible(true);
	Debug::Get()->GetText("MinerLvL")->SetIsVisible(true);
	Debug::Get()->GetText("GathererLvL")->SetIsVisible(true);

	GetScene<HUDScene>()->HideUi();

	m_isOpen = true;
}

void UiJobs::Close()
{
	if (m_isVisible)
		GetScene<HUDScene>()->ShowUi();

	Entity::SetIsVisible(false);

	m_gathererUi->SetIsVisible(false);
	m_farmerUi->SetIsVisible(false);
	m_minerUi->SetIsVisible(false);

	Debug::Get()->GetText("metier")->SetIsVisible(false);
	Debug::Get()->GetText("FarmerLvL")->SetIsVisible(false);
	Debug::Get()->GetText("MinerLvL")->SetIsVisible(false);
	Debug::Get()->GetText("GathererLvL")->SetIsVisible(false);

	m_isOpen = false;
}

void UiJobs::SetIsVisible(bool p_visible)
{
	Entity::SetIsVisible(p_visible);

	m_gathererUi->SetIsVisible(p_visible);
	m_farmerUi->SetIsVisible(p_visible);
	m_minerUi->SetIsVisible(p_visible);

	Debug::Get()->GetText("metier")->SetIsVisible(p_visible);
	Debug::Get()->GetText("FarmerLvL")->SetIsVisible(p_visible);
	Debug::Get()->GetText("MinerLvL")->SetIsVisible(p_visible);
	Debug::Get()->GetText("GathererLvL")->SetIsVisible(p_visible);

}

void UiJobs::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);

	Vector2i dimension = GetDimension();

	Vector2f farmerPos = { GetPosition({0.5f,0}).GetX() - dimension.GetX() * 0.25f,GetPosition().GetY() };
	Vector2f gathererPos = { GetPosition({0.5f,1}).GetX() + dimension.GetX() * 0.25f,  GetPosition().GetY() };
	Vector2f minerPos = GetPosition();
	Vector2f textPos = GetPosition({ 0.5f, 0 }) + Vector2f(0, 60);

	m_gathererUi->SetPosition(gathererPos);
	m_farmerUi->SetPosition(farmerPos);
	m_minerUi->SetPosition(minerPos);

	Debug::Get()->GetText("metier")->SetPosition(textPos);
}
