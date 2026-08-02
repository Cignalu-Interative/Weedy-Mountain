#include "pch.h"
#include "HUDScene.h"

#include "2DPendentLib/AssetManager.h"

#include "NightEffect.h"

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

#include "RecipesUi.h"
#include "Ui/UiShop.h"

#include "2DPendentEngine/DataStorage.h"

void HUDScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	ItemDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");
	RecipeDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");

	QuestManager::Get()->SetScene(this);

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
	NightEffect* nightEffect = CreateEntity<NightEffect>(GetCamera()->GetPosition(), GetWindowSize(), "", Shape::Rectangle, true);

	UiStats* statsUi = CreateEntity<UiStats>(uiStatsPos, { 300,150 }, "", Shape::Rectangle, true);
	UiMoney* moneyUi = CreateEntity<UiMoney>({ (float)GetWindowWidth() - 100, 50 }, { 120,64 }, "", Shape::Rectangle, true);
	UiCash* cashUi = CreateEntity<UiCash>({ (float)GetWindowWidth() - 100, 100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiTime* timeUi = CreateEntity<UiTime>({ 100,100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiDate* dateUi = CreateEntity<UiDate>({ 100,50 }, { 120,64 }, "", Shape::Rectangle, true);

	nightEffect->RemoveCollider("default");

	statsUi->RemoveCollider("default");
	moneyUi->RemoveCollider("default");
	cashUi->RemoveCollider("default");
	timeUi->RemoveCollider("default");
	dateUi->RemoveCollider("default");
	m_jobsUi = CreateEntity<UiJobs>(GetWindowCenter(), { 1000, 600 }, "", Shape::Rectangle, true);

	Inventory* inv = CreateEntity<Inventory>(GetWindowCenter(), { 636, 210 }, "InventoryWeedyMountain", Shape::Rectangle);
	inv->GetSprite()->ResizeStretched(inv->GetDimension());
	inv->SyncSprites();
	inv->RemoveCollider("default");

	Vector2f hotbarPos = { inv->GetPosition().GetX(), inv->GetPosition().GetY() + 300 };
	Hotbar* hot = CreateEntity<Hotbar>(hotbarPos, { 636, 96 }, "Hotbar", Shape::Rectangle);
	hot->SetIsVisible(false);

	m_playerInventory = new PlayerInventory(inv, hot);
	m_jobsUi->SetIsVisible(false);

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
	m_playerInventory->AddItem("ITEM_WOOD", 64, false);
	m_playerInventory->AddItem("ITEM_RAW_GOLD", 64, false);
	m_playerInventory->AddItem("ITEM_RAW_GOLD", 64, false);
	m_playerInventory->AddItem("ITEM_DIAMOND_AXE", 1, false);
	m_playerInventory->AddItem("ITEM_DIAMOND_PICKAXE", 1, false);
	m_playerInventory->AddItem("ITEM_WOODEN_PICKAXE", 1, false);
	m_recipe = CreateEntity<RecipesUi>(GetWindowCenter(), { 1200,600 }, "", Shape::Rectangle, true);
	m_recipe->SetDrawLayer(INT_MAX - 200);
	m_recipe->Close();
;
}

void HUDScene::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_I))
		m_collectionUi->Open();

	if (InputManager::Get()->IsKeyboardDown(K_O))
		m_collectionUi->Close();
	InputManager* input = InputManager::Get();
	if (input->IsKeyboardDown(K_SUPERSCRIPT_TWO))
	{
		m_questMenu->Toggle(&QuestManager::Get()->GetQuestsInProcess());
	}
	if (InputManager::Get()->IsKeyboardDown(K_J))
		m_jobsUi->Toggle();

	if (InputManager::Get()->IsKeyboardHeld(K_L))
		GameData::Get().GetPlayerData().AddXpToJob(JobType::Farmer, 5.f);
	
	if (InputManager::Get()->IsKeyboardDown(K_P))
		m_recipe->Open();

	if (InputManager::Get()->IsKeyboardDown(K_KP_0))
	{
		GameData::Get().UnlockRecipe("a");
	}
}

void HUDScene::OnUpdate()
{
	GameData& gm = GameData::Get();
	gm.Update();
	m_playerInventory->OnUpdate();
	QuestManager::Get()->UpdateQuests();
	//m_questMenu->OnUpdate();
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
