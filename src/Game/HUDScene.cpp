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
#include "SpriteContainer.h"

void HUDScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	ItemDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");
	RecipeDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");

	QuestManager::Get()->SetScene(this);

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
	nightEffect->RemoveCollider("default");
	
	Vector2f timePos = { GetWindowCenter().GetX() - GetWindowWidth()/12 ,GetWindowCenter().GetY() - (GetWindowHeight() / 2)};
	Vector2f backgroundPos = { GetWindowCenter().GetX(), GetWindowCenter().GetY() - (GetWindowHeight() / 2) };

	m_backgroundUiTimeDate = CreateEntity<Entity>(backgroundPos, { 400,200 }, "spriteTimeDateUi", Shape::Rectangle, true);
	m_backgroundUiTimeDate->GetSprite()->ResizeStretched(m_backgroundUiTimeDate->GetDimension());
	m_backgroundUiTimeDate->ShowCollider(true);
	m_backgroundUiTimeDate->SetDrawLayer(INT_MAX - 1);
	m_backgroundUiTimeDate->GetCollider()->SetIsTrigger(true);
	m_backgroundUiTimeDate->SyncSprites();

	m_statsUi = CreateEntity<UiStats>({}, {300,150}, "", Shape::Rectangle, true);
	m_moneyUi = CreateEntity<UiMoney>({}, { 160,64 }, "", Shape::Rectangle, true);
	m_cashUi = CreateEntity<UiCash>({}, { 160,64 }, "", Shape::Rectangle, true);
	m_timeUi = CreateEntity<UiTime>({}, { 120,64 }, "", Shape::Rectangle, true);
	m_dateUi = CreateEntity<UiDate>({}, { 120,64 }, "", Shape::Rectangle, true);



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

	UiPos();
	BarButtonInit();
}

void HUDScene::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_I))
		m_collectionUi->Open();

	if (InputManager::Get()->IsKeyboardDown(K_O))
		m_collectionUi->Close();
	InputManager* input = InputManager::Get();
	if (input->IsKeyboardDown(K_W))
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
	Vector2i newSize = GetWindowSize();

	if(newSize != m_lastWindowSize)
	{
		m_lastWindowSize = newSize;
		UiPos();
	}

	GameData& gm = GameData::Get();
	gm.Update();
	m_playerInventory->OnUpdate();
	QuestManager::Get()->UpdateQuests();
	//m_questMenu->OnUpdate();
}

void HUDScene::OnDestroy()
{

}

void HUDScene::UiPos()
{

	float topRightPos = (float)GetWindowWidth();
	float bottomRightPos = (float)GetWindowHeight();

	Vector2f uiStatsPos = { topRightPos - GetWindowWidth() / 12, bottomRightPos - GetWindowHeight() / 11 };

	Vector2f moneyPos = { topRightPos - GetWindowWidth() / 12, 50 };
	Vector2f cashPos = { topRightPos - GetWindowWidth() / 12, 100 };
	Vector2f backgroundPos = { GetWindowCenter().GetX(), GetWindowCenter().GetY() - (GetWindowHeight() / 2) };
	Vector2f timePos = { GetWindowCenter().GetX() - 106 ,GetWindowCenter().GetY() - (GetWindowHeight() / 2) };
	Vector2f datePos = { GetWindowCenter().GetX() + 106 ,GetWindowCenter().GetY() - (GetWindowHeight() / 2) };

	
	//GameData::Get().GetPlayerData().m_playerInventory->GetInventory()->SetPosition(GetWindowCenter());
	//Inventory* inv = GameData::Get().GetPlayerData().m_playerInventory->GetInventory();
	//Vector2f hotbarPos = { GetWindowCenter().GetX(),(float)GetWindowHeight() / 2 };

	//GameData::Get().GetPlayerData().m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	m_backgroundUiTimeDate->SetPosition(backgroundPos);
	m_statsUi->SetPosition(uiStatsPos);
	m_moneyUi->SetPosition(moneyPos);
	m_cashUi->SetPosition(cashPos);
	m_timeUi->SetPosition(timePos);
	m_dateUi->SetPosition(datePos);
	//m_statsUi = CreateEntity<UiStats>(uiStatsPos, { 300,150 }, "", Shape::Rectangle, true);
	//m_moneyUi = CreateEntity<UiMoney>(moneyPos, { 160,64 }, "", Shape::Rectangle, true);
	//m_cashUi = CreateEntity<UiCash>(cashPos, { 160,64 }, "", Shape::Rectangle, true);
}

void HUDScene::BarButtonInit()
{
	Vector2f entityPos = { GetWindowCenter().GetX() - GetWindowWidth() / 2, GetWindowCenter().GetY()};
	m_barButtonEntity = CreateEntity<Entity>(entityPos , {30,128}, "InventoryWeedyMountain", Shape::Rectangle, true);
	m_barButtonEntity->GetSprite()->ResizeStretched(m_barButtonEntity->GetDimension());
	m_barButtonEntity->SyncSprites();
	m_barButtonEntity->RemoveCollider("default");
	m_barButtonEntity->SetDrawLayer(INT_MAX - 1);

	Vector2i dim = { 64 ,32 };
	Vector2f topLeftPos = m_barButtonEntity->GetPosition({}) + Vector2f{ 16,16 };
	

	m_jobsButton = CreateButton(topLeftPos, dim, "button", "", "");
	m_jobsButton->AddSprite("jobsIcon", "poleEmploi", {16,16}, 1, {16,0}, true);
	m_jobsButton->GetSprite("jobsIcon")->ResizeStretched({ 64,32 });
	m_jobsButton->GetSprite()->ResizeStretched(m_jobsButton->GetDimension());
	m_jobsButton->SyncSprites();
	m_jobsButton->SetOnHoverStay([this] {m_jobsButton->SetSprite("button2"); });
	m_jobsButton->SetOnHoverExit([this] {m_jobsButton->SetSprite("button"); });
	m_jobsButton->SetOnLeftClick([this] {m_jobsUi->Toggle(); });
	m_jobsButton->ShowCollider(true);


	m_questButton = CreateButton(topLeftPos + Vector2f{ 0,32 }, dim, "button", "", "");
	m_questButton->AddSprite("questIcon", "parchment", { 16,16 }, 1, { 16,0 }, true);
	m_questButton->GetSprite("questIcon")->ResizeStretched({ 32,32 });
	m_questButton->GetSprite()->ResizeStretched(m_questButton->GetDimension());
	m_questButton->SyncSprites();
	m_questButton->SetOnHoverStay([this] {m_questButton->SetSprite("button2"); });
	m_questButton->SetOnHoverExit([this] {m_questButton->SetSprite("button"); });
	m_questButton->SetOnLeftClick([this] {m_questMenu->Open(&QuestManager::Get()->GetQuestsInProcess()); });

	m_recipiesButton = CreateButton(topLeftPos + Vector2f{ 0,64 }, dim, "button", "", "");
	m_recipiesButton->GetSprite()->ResizeStretched(m_recipiesButton->GetDimension());
	m_recipiesButton->AddSprite("recipiesIcon", "craftingTable", { 16,16 }, 1, { 16,0 }, true);
	m_recipiesButton->GetSprite("recipiesIcon")->ResizeStretched({ 16,16 });
	m_recipiesButton->SyncSprites();
	m_recipiesButton->SetOnHoverStay([this] {m_recipiesButton->SetSprite("button2"); });
	m_recipiesButton->SetOnHoverExit([this] {m_recipiesButton->SetSprite("button"); });
	m_recipiesButton->SetOnLeftClick([this] {m_recipe->Open(); });

	m_collectionButton = CreateButton(topLeftPos + Vector2f{ 0,96 }, dim, "button", "", "");
	m_collectionButton->AddSprite("collectionIcon", "book", { 16,16 }, 1, { 16,0 }, true);
	m_collectionButton->GetSprite("collectionIcon")->ResizeStretched({ 16,16 });
	m_collectionButton->GetSprite()->ResizeStretched(m_collectionButton->GetDimension());
	m_collectionButton->SyncSprites();
	m_collectionButton->SetOnHoverStay([this] {m_collectionButton->SetSprite("button2"); });
	m_collectionButton->SetOnHoverExit([this] {m_collectionButton->SetSprite("button"); });
	m_collectionButton->SetOnLeftClick([this] {m_collectionUi->Open(); });
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
