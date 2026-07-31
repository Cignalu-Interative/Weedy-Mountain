#include "pch.h"
#include "HUDScene.h"

#include "2DPendentLib/AssetManager.h"

#include "Ui/UiStats.h"
#include "Ui/UiMoney.h"
#include "Ui/UiCash.h"
#include "Ui/UiDate.h"
#include "Ui/UiTime.h"
#include "Ui/UiJobs.h"
#include "UiCollection.h"

#include "GameData.h"
#include "PlayerInventory.h"
#include "2DPendentLib/InputManager.h"
#include "ItemDataBase.h"
#include "RecipeDataBase.h"

#include "QuestManager.h"
#include "QuestMenu.h"
#include "Furnace.h"
#include "UiChest.h"

void HUDScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	ItemDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");
	RecipeDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");

	QuestManager::Get()->SetScene(this);

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
	hot->SetIsVisible(false);

	m_playerInventory = new PlayerInventory(inv, hot);
	m_playerInventory->AddItem("ITEM_WOOD", 1028, false);

	GameData::Get().SetInventory(m_playerInventory);
	m_collectionUi = CreateEntity<UiCollection>(GetWindowCenter(), {1000,600}, "", Shape::Rectangle, true);

	SceneManager::Get()->SetPersistentScene(this);

	QuestMenu* questMenu = CreateEntity<QuestMenu>({ (float)GetWindowWidth() / 2.0f, (float)GetWindowHeight() / 2.0f }, { (int)(GetWindowWidth() * 0.85f), (int)(GetWindowHeight() * 0.85f) }, "InventoryWeedyMountain", Shape::Rectangle, true);
	questMenu->GetSprite()->ResizeStretched(questMenu->GetDimension());
	questMenu->SyncSprites();
	questMenu->SetDrawLayer(INT_MAX - 20);
	questMenu->GetCollider()->SetIsTrigger(true);
	//questMenu->GetCollider()->SetShow(true);
	m_questMenu = questMenu;
	m_playerInventory->AddItem("ITEM_WOOD", 25, false);
	m_playerInventory->AddItem("ITEM_WOOD", 64, false);
	m_playerInventory->AddItem("ITEM_RAW_GOLD", 64, false);
	m_playerInventory->AddItem("ITEM_RAW_GOLD", 64, false);
	m_playerInventory->AddItem("ITEM_FIBER", 25, false);

}

void HUDScene::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_KP_8))
		m_collectionUi->Open();

	if (InputManager::Get()->IsKeyboardDown(K_KP_7))
		m_collectionUi->Close();
	InputManager* input = InputManager::Get();
	if (input->IsKeyboardDown(KeyboardKeycodeKeys::K_SUPERSCRIPT_TWO))
	{
		m_questMenu->Toggle(&QuestManager::Get()->GetQuestsInProcess());
	}
}

void HUDScene::OnUpdate()
{
	GameData& gm = GameData::Get();
	gm.Update();
	m_playerInventory->OnUpdate();
	QuestManager::Get()->UpdateQuests();
	m_questMenu->OnUpdate();
}

void HUDScene::OnDestroy()
{

}
