#include "pch.h"
#include "GameScene.h"
#include "EnviroProps.h"
#include "EnviroSpawn.h"

#include "ItemOnGround.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "ItemID.h"
#include "ItemStack.h"

#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

#include "FarmTile.h"
#include "FarmManager.h"
#include "GameData.h"
#include "2DPendentLib/Debug.h" 

#include "PNJ.h"
#include "Ui/UiShop.h"
#include "SpriteContainer.h"

#include "Ui/UiStats.h"
#include "Ui/UiMoney.h"
#include "Ui/UiCash.h"
#include "Ui/UiDate.h"
#include "Ui/UiTime.h"
#include "Ui/UiJobs.h"
#include "Ui/UiGatherer.h"
#include "TextBox.h"

#include "2DPendentLib/ObjectGroupTiled.h"
#include "2DPendentLib/ObjectTiled.h"

#include "SoundManager.h"

#include "Aleatory.h"

#include "QuestManager.h"

void GameScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	SoundManager::Get()->AddSound("../../res/Game/Assets/Sounds/Sounds_Repertory.json");
	MapTiledManager* mm = MapTiledManager::Get();
	mm->AddMaps("../../res/Game/Assets/Maps/Maps_Repertory.json");
	mm->SetCurrentMap("Farm");

	std::vector<ObjectTiled*> a = mm->GetCurrentMap()->GetObjectGroupByName("Object")->GetObjectTiledByName("Player Spawn");

	for (ObjectTiled* o : a)
	{
		m_spawnPoint = o->GetObjectPosition();
	}

	GameData& gm = GameData::Get();
	gm.SetTimeScale(100);
	gm.Initialize();

	ItemDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");
	Application::Get()->SetFPSUserWanted(10000);
	Application::Get()->SetIsAutoFPS(false);

	GetCamera()->SetPosition(m_spawnPoint);

	m_player = CreateEntity<Player>(m_spawnPoint, { 16,32 }, "player", Shape::Rectangle);
	m_player->GetCollider()->SetDimension({ 14,14 });
	m_player->GetCollider()->SetOffset({ 0,9 });
	m_player->SetRigidBody(true);
	m_player->SetTag(1);
	m_player->SetDrawLayer(2);
	m_player->GetCollider()->SetLayer(PLAYER);
	m_player->AddCollider("actionBox", { 16,16 }, Shape::Rectangle, { 0,0 }, true);
	m_player->AddCollider("interact", { 80,80 }, Shape::Circle, { 0,0 }, true);
	m_player->ShowCollider(true);
	m_player->InitialisePhysicsBody(ShapeBodyPhysics::Dynamic);
	m_player->GetPhysicsBody()->SetGravityEnabled(false);
	m_player->GetPhysicsBody()->SetLinearDamping(0.f);
	m_player->GetPhysicsBody()->SetRestitution(0.f);
	//
	GetCamera()->SetZoom(5.f);
	GetCamera()->SetFollowedEntity(m_player);
	GetCamera()->SetSmoothSpeed(2);
	Application::Get()->SetClampCameraToCurrentMap(true);

	MapTiled* map = mm->GetCurrentMap();

	std::vector<ObjectTiled*> objectGroup = map->GetObjectGroupByName("Wall")->GetObjectTiledByName("Wall");

	for (ObjectTiled* obj : objectGroup)
	{
		Vector2f dim = obj->GetObjectDimension();
		Entity* ent = CreateEntity<Entity>(obj->GetObjectPosition() + dim/2, (Vector2i)dim, "", Shape::Rectangle); 
	}

	objectGroup = map->GetObjectGroupByName("Wall")->GetObjectTiledByName("Water");

	for (ObjectTiled* obj : objectGroup)
	{
		Vector2f dim = obj->GetObjectDimension();
		Entity* ent = CreateEntity<Entity>(obj->GetObjectPosition() + dim / 2, (Vector2i)dim, "", Shape::Rectangle);
		ent->SetTag(5);
	}

	std::vector<TileInfo> canGrowTiles = map->GetAllTilesWithProperty("CanGrow", "CanGrow");
	int size = canGrowTiles.size();
	Vector2i tileDim = map->GetTileDimension();

	for (const TileInfo& info : canGrowTiles)
	{
		Vector2f center = {
			info.worldPos.GetX() + tileDim.GetX() / 2.f,
			info.worldPos.GetY() + tileDim.GetY() / 2.f
		};
		FarmTile* tile = CreateEntity<FarmTile>(center, tileDim, "", Shape::Rectangle, false);
		tile->GetCollider()->SetLayer(FARMTILE);
		tile->GetCollider()->SetMask(NoNE);
		//tile->ShowCollider(true);
		FarmManager::Get()->RegisterTile(tile);
	}


	/*UiJobs* j = CreateEntity<UiJobs>(m_player->GetPosition(), { 1000, 600 }, "", Shape::Rectangle, true);
	j->ShowCollider(true);*/
	UiMoney* f = CreateEntity<UiMoney>({ (float)GetWindowWidth() - 100, 50}, {120,64}, "", Shape::Rectangle, true);
	//f->ShowCollider(true);
	UiCash* g = CreateEntity<UiCash>({ (float)GetWindowWidth() - 100, 100 }, { 120,64 }, "", Shape::Rectangle, true);
	//f->ShowCollider(true);
	UiTime* h = CreateEntity<UiTime>({ 100,100 }, { 120,64 }, "", Shape::Rectangle, true);
	UiDate* i = CreateEntity<UiDate>({ 100,50 }, { 120,64 }, "", Shape::Rectangle, true);

	/*UiJobs* j = CreateEntity<UiJobs>(m_player->GetPosition(), { 1000, 600 }, "", Shape::Rectangle, true);
	j->ShowCollider(true);*/

	// CreateText("KAKA", { 500, 500 }, "KAKA KAKA KAKA KAKA KAKA", ColorsNamed::LightIndigo, true);
	// Text* t = Debug::Get()->GetText("KAKA");
	// t->SetFont("Gameplay", 55);
	// t->SetText("KAKOU KAKOU KAKOU KAKOU KAKOU KAKOU KAKOU KAKOU KAKOU KAKOU KAKOU KAKOU");
	// t->SetFontSize(60);
	// t->SetColor(ColorsNamed::Violet);
	// t->SetWrapWidth(600);
	// t->SetFontStyle(false, true);
	// t->SetOutlineSize(2);
	// t->SetOutLineColor(ColorsNamed::White);
	// t->SetLineSpacing(50);
	// t->HorizontalAlignText(HorizontalAlignment::Center);

	//m_player2 = CreateEntity<Player>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 16,32 }, "player", Shape::Rectangle);
	//m_player2->GetInventory()->AddItem("ITEM_WOOD", 300, false);
	//m_player2->GetInventory()->AddItem("ITEM_FIBER", 30, false);

	//UiShop* a = CreateEntity<UiShop>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 1000, 600 }, "InventoryWeedyMountain", Shape::Rectangle, true);
	//a->GetSprite()->SetDimension(a->GetDimension());
	//a->SyncSprites();
	////a->ShowCollider(true);
	CreateText("time", { 200, 50 }, std::to_string(gm.GetHour()) + ":" + std::to_string(gm.GetMinute()), { 255, 255, 255, 255 }, true);

	EnviroSpawn::Get()->InitEnviroSpawn();
	//EnviroSpawn::Get()->InitEnviroSpawn();

// 	m_textBox = CreateTextBox("TextBox", { 150, 150 }, { 250, 200 }, ColorsNamed::DarkBlue, true);
// 	m_textBox->GetCollider()->SetIsTrigger(true);
// 	m_textBox->SetBackgroundFilled(true);

	//Application::Get()->GetWindow()->SetWindowAlwaysOnTop(true);

	QuestManager::Get()->CreateQuest("Quest", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);
	QuestManager::Get()->GetQuestOnNotStarted("Quest")->AddRequirement(CheckQuest::AddItem, "Item_Wood", 1);
	QuestManager::Get()->GetQuestOnNotStarted("Quest")->AddReward(ItemDataBase::Get()->GetItem("ITEM_EGG"), 67);
	QuestManager::Get()->GetQuestOnNotStarted("Quest")->SetHistoryDescription("This is a test quest to check if the quest system is working properly. You need to collect 1 wood and you will be rewarded with 67 eggs.");
	
	//QuestManager::Get()->GetQuestOnNotStarted("Quest")->AddRequirement(CheckQuest::AddItem, "Item_Rock", 1);
	//QuestManager::Get()->GetQuestOnNotStarted("Quest")->AddRequirement(CheckQuest::AddItem, "Item_Fiber", 5);

	QuestManager::Get()->CreateQuest("Quest KAKA", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);
	QuestManager::Get()->CreateQuest("KAKA Quest", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);
	QuestManager::Get()->CreateQuest(" KAKA Quest KAKA", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);
	QuestManager::Get()->CreateQuest(" KAKOU Quest KAKI", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);
	QuestManager::Get()->CreateQuest(" KAKIKOU LE CAILLOU", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);
	QuestManager::Get()->CreateQuest(" ROGER LE CASTOR", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);
	QuestManager::Get()->CreateQuest(" CASTOR-AMA", { (float)GetWindowWidth() - 30, 180.f }, { 150, 69 }, m_player);

}


void GameScene::OnEvent()
{
	InputManager* im = InputManager::Get();

	if (im->IsKeyboardDown(K_A))
		GetCamera()->SetZoom(5);

	if (im->IsKeyboardDown(K_E))
		GetCamera()->SetZoom(1);

	if (InputManager::Get()->IsKeyboardDown(K_KP_1))
	{
		Application::Get()->SetIsAutoFPS(false);
	}
	if (InputManager::Get()->IsKeyboardDown(K_KP_2))
	{
		Application::Get()->SetIsAutoFPS(true);
	}
	//if (im->IsKeyboardDown(K_K))
	//{
	//	m_player->GetJobByName(JobType::Gatherer)->m_exp += 5;
	//	m_player->GetJobByName(JobType::Miner)->m_exp += 5;
	//	m_player->GetJobByName(JobType::Farmer)->m_exp += 5;
	//}

	//if (im->IsKeyboardDown(K_O))
	//{
	//	m_player->GetJobByName(JobType::Gatherer)->m_exp -= 5;
	//	m_player->GetJobByName(JobType::Miner)->m_exp -= 5;
	//	m_player->GetJobByName(JobType::Farmer)->m_exp -= 5;
	//}

	if (im->IsKeyboardDown(K_R))
	{
		
		SceneManager::Get()->PushScene("village");
	}

	if (im->IsKeyboardDown(K_M))
		m_player->TakeDamage(5);

	if (im->IsKeyboardDown(K_O))
		GameData::Get().GetPlayerData().m_playerInventory->AddItem("ITEM_WOOD", 64, false);


	// if (im->IsMouseDown(BUTTON_LEFT) && m_textBox->IsMouseIn())
	// {
	// 	im->StartTextInput();
	// }
	// if (im->IsMouseDown(BUTTON_RIGHT	))
	// {
	// 	im->StopTextInput();
	// }
	// if (im->IsTextInputActive() == true && im->IsKeyboardDown(K_DELETE))
	// {
	// 	im->DeleteLastCharInFinalTextInput();
	// }
	// if (im->IsTextInputActive() == true)
	// {
	// 	//m_textBox->SetText(im->GetfinalTextInput());
	// 	m_textBox->SetTextInBox(im->GetfinalTextInput());
	// }
	// std::string t = im->GetfinalTextInput();
	//std::cerr << "TextInput : " << t << std::endl;
}

void GameScene::OnUpdate()
{
	FarmManager::Get()->Update();
	EnviroSpawn::Get()->Update();
	GameData& gm = GameData::Get();
	gm.Update();
	CreateText("money", { 500, 10 }, "Money : " + std::to_string(m_player->GetMoney()), ColorsNamed::White, true);
	Debug::Get()->GetText("time")->SetText(std::to_string(gm.GetHour()) + ":" + std::to_string(gm.GetMinute()));
	//std::cout << "X = " << m_Player->GetPosition().GetX() / 2 << " Y = " << m_Player->GetPosition().GetY() / 2 << std::endl;

	SoundManager* sm = SoundManager::Get();

	switch (gm.GetMonth())
	{
	case 1:
	{
		sm->GetSound("L_hiver (Jour) - OST from Weedy Mountain v1")->Stop();

		if (gm.GetHour() > 20 || gm.GetHour() < 6)
		{
			Sound* sound = sm->GetSound("L_Automne (Nuit) - OST from Weedy Mountain v1");
			if (sound->IsPlaying())
			{
				return;
			}
			else
			{
				sm->GetSound("L_Automne (Jour) - OST from Weedy Mountain v1")->Stop();
				sound->SetLoopCount(-1);
				sound->SetStartAtTime(Aleatory::GetInt(0, 30));
				sound->SetLoopStartAtTime(Aleatory::GetInt(0, 30));
				sound->SetVolumeFadeIn(2, 10);
				sound->SetSpacializationEnabled(false);
				sound->Play();
			}
		}
		else
		{
			Sound* sound = sm->GetSound("L_Automne (Jour) - OST from Weedy Mountain v1");
			if (sound->IsPlaying())
			{
				return;
			}
			else
			{
				sm->GetSound("L_Automne (Nuit) - OST from Weedy Mountain v1")->Stop();
				sound->SetLoopCount(-1);
				sound->SetStartAtTime(Aleatory::GetInt(0, 60));
				sound->SetLoopStartAtTime(Aleatory::GetInt(0, 60));
				sound->SetVolumeFadeIn(2, 10);
				sound->SetSpacializationEnabled(false);
				sound->Play();
			}
		}
		break;
	}
	case 2:
	{

		break;
	}
	case 3:
	{
		
		break;
	}
	case 4:
	{
		sm->GetSound("L_Automne (Jour) - OST from Weedy Mountain v1")->Stop();
		sm->GetSound("L_Automne (Nuit) - OST from Weedy Mountain v1")->Stop();

		if (gm.GetHour() > 20 || gm.GetHour() < 6)
		{
			/*Sound* sound = sm->GetSound("");
			if (sound->IsPlaying())
			{
				return;
			}
			else
			{
				sm->GetSound("L_hiver (Jour) - OST from Weedy Mountain v1")->Stop();
				sound->SetLoopCount(-1);
				sound->SetStartAtTime(Aleatory::GetInt(0, 30));
				sound->SetLoopStartAtTime(Aleatory::GetInt(0, 30));
				sound->SetVolumeFadeIn(2, 10);
				sound->SetSpacializationEnabled(false);
				sound->Play();
			}*/
		}
		else
		{
			Sound* sound = sm->GetSound("L_hiver (Jour) - OST from Weedy Mountain v1");
			if (sound->IsPlaying())
			{
				return;
			}
			else
			{
				//sm->GetSound("")->Stop();
				sound->SetLoopCount(-1);
				sound->SetStartAtTime(Aleatory::GetInt(0, 60));
				sound->SetLoopStartAtTime(Aleatory::GetInt(0, 60));
				sound->SetVolumeFadeIn(2, 10);
				sound->SetSpacializationEnabled(false);
				sound->Play();
			}
		}
		break;
	}
	}
}

void GameScene::OnDestroy()
{
}

void GameScene::OnSleep()
{
	//SetSleepUpdate(true); 
	SetSleepDraw(true);
	SetSleepEvent(true);
	m_player->SetIsVisible(false);
	
	//for (Entity* e : GetEntities())
	//{
	//	e->SetIsVisible(false);
	//}

	//e->SetIsVisible(true);
	//f->SetIsVisible(true);
	//g->SetIsVisible(true);
	//h->SetIsVisible(true);
	//i->SetIsVisible(true);

}

void GameScene::OnWake()
{
	SetSleepDraw(false);
	SetSleepEvent(false);
	m_player->SetIsVisible(true);
	GetCamera()->SetPosition(m_player->GetPosition());
	GetCamera()->SetFollowedEntity(m_player);


	//m_player->SetPosition(m_spawnPoint);
	//GetCamera()->SetPosition(m_spawnPoint);

	MapTiledManager* mm = MapTiledManager::Get();
	mm->SetCurrentMap("Farm");
}

void GameScene::OnDraw()
{
}
