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

#include "QuestManager.h"
#include "QuestMenu.h"

void HUDScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	QuestManager::Get()->SetScene(this);

	Vector2f uiStatsPos = { 1175, 600 };

	//UiStats* statsUi = CreateEntity<UiStats>(uiStatsPos, { 300,150 }, "", Shape::Rectangle, true);
	UiMoney* moneyUi = CreateEntity<UiMoney>({ (float)GetWindowWidth() - 100, 50 }, { 120,64 }, "", Shape::Rectangle, true);
	UiCash* cashUi = CreateEntity<UiCash>({ (float)GetWindowWidth() - 100, 100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiTime* timeUi = CreateEntity<UiTime>({ 100,100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiDate* dateUi = CreateEntity<UiDate>({ 100,50 }, { 120,64 }, "", Shape::Rectangle, true);

	Inventory* inv = CreateEntity<Inventory>(GetWindowCenter(), { 636, 210 }, "InventoryWeedyMountain", Shape::Rectangle);
	inv->GetSprite()->ResizeStretched(inv->GetDimension());
	inv->SyncSprites();

	Vector2f hotbarPos = { inv->GetPosition().GetX(), inv->GetPosition().GetY() + 300 };
	Hotbar* hot = CreateEntity<Hotbar>(hotbarPos, { 636, 96 }, "Hotbar", Shape::Rectangle);
	hot->SetIsVisible(false);

	m_playerInventory = new PlayerInventory(inv, hot);

	GameData::Get().SetInventory(m_playerInventory);
	SceneManager::Get()->SetPersistentScene(this);

	QuestMenu* questMenu = CreateEntity<QuestMenu>({ (float)GetWindowWidth() / 2.0f, (float)GetWindowHeight() / 2.0f }, { (int)(GetWindowWidth() * 0.85f), (int)(GetWindowHeight() * 0.85f) }, "InventoryWeedyMountain", Shape::Rectangle, true);
	questMenu->GetSprite()->ResizeStretched(questMenu->GetDimension());
	questMenu->SyncSprites();
	questMenu->SetDrawLayer(INT_MAX - 20);
	questMenu->GetCollider()->SetIsTrigger(true);
	questMenu->GetCollider()->SetShow(true);



}

void HUDScene::OnEvent()
{

}

void HUDScene::OnUpdate()
{
	m_playerInventory->OnUpdate();
	QuestManager::Get()->UpdateQuests();
}

void HUDScene::OnDestroy()
{

}
