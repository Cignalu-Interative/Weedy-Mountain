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


void GameScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	MapTiledManager* mm = MapTiledManager::Get();
	mm->AddMaps("../../res/Game/Assets/Maps/Maps_Repertory.json");
	mm->SetCurrentMap("Farm");

	ObjectGroupTiled* layer = mm->GetCurrentMap()->GetObjectGroupByName("Object");
	std::vector<ObjectTiled*> object = layer->GetAllObjects();

	for (ObjectTiled* o : object)
	{
		if (o->GetObjectName() == "Player Spawn")
		{
			m_spawnPoint = o->GetObjectPosition();
			break;
		}
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

	//m_player2 = CreateEntity<Player>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 16,32 }, "player", Shape::Rectangle);
	//m_player2->GetInventory()->AddItem("ITEM_WOOD", 300, false);
	//m_player2->GetInventory()->AddItem("ITEM_FIBER", 30, false);

	//UiShop* a = CreateEntity<UiShop>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 1000, 600 }, "InventoryWeedyMountain", Shape::Rectangle, true);
	//a->GetSprite()->SetDimension(a->GetDimension());
	//a->SyncSprites();
	////a->ShowCollider(true);
	//CreateText("time", { 200, 50 }, std::to_string(gm.GetHour()) + ":" + std::to_string(gm.GetMinute()), { 255, 255, 255, 255 }, true);

	EnviroSpawn::Get()->InitEnviroSpawn();
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

}

void GameScene::OnUpdate()
{
	FarmManager::Get()->Update();
	EnviroSpawn::Get()->Update();
	GameData& gm = GameData::Get();
	gm.Update();
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
