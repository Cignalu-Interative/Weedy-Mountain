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

#include "RecipesUi.h"
#include "RecipeDataBase.h"
#include "ItemDataBase.h"

void HUDScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	ItemDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");
	RecipeDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");
	Vector2f uiStatsPos = { 1175, 600 };

	m_statsUi = CreateEntity<UiStats>(uiStatsPos, { 300,150 }, "", Shape::Rectangle, true);
	m_moneyUi = CreateEntity<UiMoney>({ (float)GetWindowWidth() - 100, 50 }, { 120,64 }, "", Shape::Rectangle, true);
	m_cashUi = CreateEntity<UiCash>({ (float)GetWindowWidth() - 100, 100 }, { 120,64 }, "", Shape::Rectangle, true);
	m_timeUi = CreateEntity<UiTime>({ 100,100 }, { 120,64 }, "", Shape::Rectangle, true);
	m_dateUi = CreateEntity<UiDate>({ 100,50 }, { 120,64 }, "", Shape::Rectangle, true);

	m_statsUi->RemoveCollider("default");
	m_moneyUi->RemoveCollider("default");
	m_cashUi->RemoveCollider("default");
	m_timeUi->RemoveCollider("default");
	m_dateUi->RemoveCollider("default");

	Inventory* inv = CreateEntity<Inventory>(GetWindowCenter(), { 636, 210 }, "InventoryWeedyMountain", Shape::Rectangle);
	inv->GetSprite()->ResizeStretched(inv->GetDimension());
	inv->SyncSprites();
	inv->RemoveCollider("default");

	Vector2f hotbarPos = { inv->GetPosition().GetX(), inv->GetPosition().GetY() + 300 };
	Hotbar* hot = CreateEntity<Hotbar>(hotbarPos, { 636, 96 }, "Hotbar", Shape::Rectangle);

	m_playerInventory = new PlayerInventory(inv, hot);
	m_playerInventory->AddItem("ITEM_WOOD", 25, false);
	m_playerInventory->AddItem("ITEM_FIBER", 25, false);

	GameData::Get().SetInventory(m_playerInventory);
	SceneManager::Get()->SetPersistentScene(this);

	m_recipe = CreateEntity<RecipesUi>(GetWindowCenter(), { 1200,600 }, "", Shape::Rectangle, true);
	m_recipe->ShowCollider(true);
	m_recipe->Close();

}

void HUDScene::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_P))
		m_recipe->Open();
	if (InputManager::Get()->IsKeyboardDown(K_R))
		m_recipe->Close();
}

void HUDScene::OnUpdate()
{
	m_playerInventory->OnUpdate();

}

void HUDScene::OnDestroy()
{

}

void HUDScene::HideUi()
{
	m_statsUi->SetIsVisible(false);
	m_moneyUi->SetIsVisible(false);
	m_timeUi->SetIsVisible(false);
	m_cashUi->SetIsVisible(false);
	m_dateUi->SetIsVisible(false);
}

void HUDScene::ShowUi()
{
	m_statsUi->SetIsVisible(true);
	m_moneyUi->SetIsVisible(true);
	m_timeUi->SetIsVisible(true);
	m_cashUi->SetIsVisible(true);
	m_dateUi->SetIsVisible(true);
}
