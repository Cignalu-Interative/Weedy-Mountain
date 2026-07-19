#include "pch.h"
#include "HUDScene.h"

#include "2DPendentLib/AssetManager.h"

#include "Ui/UiStats.h"
#include "Ui/UiMoney.h"
#include "Ui/UiCash.h"
#include "Ui/UiDate.h"
#include "Ui/UiTime.h"
#include "Ui/UiJobs.h"

#include "GameData.h"
#include "PlayerInventory.h"
#include "2DPendentLib/InputManager.h"

void HUDScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");

	Vector2f uiStatsPos = { 1175, 600 };

	PlayerJob* gatherer = new PlayerJob();
	gatherer->m_name = JobType::Gatherer;
	GameData::Get().GetPlayerData().m_jobs.push_back(gatherer);

	PlayerJob* farmer = new PlayerJob();
	farmer->m_name = JobType::Farmer;
	GameData::Get().GetPlayerData().m_jobs.push_back(farmer);

	PlayerJob* miner = new PlayerJob();
	miner->m_name = JobType::Miner;
	GameData::Get().GetPlayerData().m_jobs.push_back(miner);

	UiStats* statsUi = CreateEntity<UiStats>(uiStatsPos, { 300,150 }, "", Shape::Rectangle, true);
	UiMoney* moneyUi = CreateEntity<UiMoney>({ (float)GetWindowWidth() - 100, 50 }, { 120,64 }, "", Shape::Rectangle, true);
	UiCash* cashUi = CreateEntity<UiCash>({ (float)GetWindowWidth() - 100, 100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiTime* timeUi = CreateEntity<UiTime>({ 100,100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiDate* dateUi = CreateEntity<UiDate>({ 100,50 }, { 120,64 }, "", Shape::Rectangle, true);
	m_jobsUi = CreateEntity<UiJobs>(GetWindowCenter(), { 1000, 600 }, "", Shape::Rectangle, true);

	Inventory* inv = CreateEntity<Inventory>(GetWindowCenter(), { 636, 210 }, "InventoryWeedyMountain", Shape::Rectangle);
	inv->GetSprite()->ResizeStretched(inv->GetDimension());
	inv->SyncSprites();

	Vector2f hotbarPos = { inv->GetPosition().GetX(), inv->GetPosition().GetY() + 300 };
	Hotbar* hot = CreateEntity<Hotbar>(hotbarPos, { 636, 96 }, "Hotbar", Shape::Rectangle);

	m_playerInventory = new PlayerInventory(inv, hot);
	m_jobsUi->SetIsVisible(false);

	GameData::Get().SetInventory(m_playerInventory);
	SceneManager::Get()->SetPersistentScene(this);
}

void HUDScene::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_J))
		m_jobsUi->Toggle();

	if (InputManager::Get()->IsKeyboardHeld(K_L))
		GameData::Get().GetPlayerData().AddXpToJob(JobType::Farmer, 5.f);
}

void HUDScene::OnUpdate()
{
	m_playerInventory->OnUpdate();

}

void HUDScene::OnDestroy()
{

}
