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
#include "UiChest.h"
#include "ItemDataBase.h"

void HUDScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	ItemDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");
	Vector2f uiStatsPos = { 1175, 600 };

	UiStats* statsUi = CreateEntity<UiStats>(uiStatsPos, { 300,150 }, "", Shape::Rectangle, true);
	UiMoney* moneyUi = CreateEntity<UiMoney>({ (float)GetWindowWidth() - 100, 50 }, { 120,64 }, "", Shape::Rectangle, true);
	UiCash* cashUi = CreateEntity<UiCash>({ (float)GetWindowWidth() - 100, 100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiTime* timeUi = CreateEntity<UiTime>({ 100,100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiDate* dateUi = CreateEntity<UiDate>({ 100,50 }, { 120,64 }, "", Shape::Rectangle, true);

	statsUi->RemoveCollider("default");
	moneyUi->RemoveCollider("default");
	cashUi->RemoveCollider("default");
	timeUi->RemoveCollider("default");
	dateUi->RemoveCollider("default");


	Inventory* inv = CreateEntity<Inventory>(GetWindowCenter(), { 636, 210 }, "InventoryWeedyMountain", Shape::Rectangle);
	inv->GetSprite()->ResizeStretched(inv->GetDimension());
	inv->SyncSprites();
	inv->RemoveCollider("default");

	Vector2f hotbarPos = { inv->GetPosition().GetX(), inv->GetPosition().GetY() + 300 };
	Hotbar* hot = CreateEntity<Hotbar>(hotbarPos, { 636, 96 }, "Hotbar", Shape::Rectangle);

	m_playerInventory = new PlayerInventory(inv, hot);
	m_playerInventory->AddItem("ITEM_WOOD", 1028, false);

	GameData::Get().SetInventory(m_playerInventory);
	SceneManager::Get()->SetPersistentScene(this);
}

void HUDScene::OnEvent()
{

}

void HUDScene::OnUpdate()
{
	m_playerInventory->OnUpdate();

}

void HUDScene::OnDestroy()
{

}
