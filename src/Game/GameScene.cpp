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
#include "UiShop.h"
void GameScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	//MapTiledManager* mm = MapTiledManager::Get();
	//mm->AddMaps("../../res/Game/Assets/Maps/Maps_Repertory.json");
	//mm->SetCurrentMap("Farm");

	GameData& gm = GameData::Get();
	gm.SetTimeScale(100);
	gm.Initialize();

	ItemDataBase::Get()->LoadFromJSON("../../res/Game/Assets/db/items_db.json");

	m_player = CreateEntity<Player>({ GetWindowWidth()/2.f, GetWindowHeight()/2.f}, {16,32}, "player", Shape::Rectangle);
	m_player->GetCollider()->SetDimension({ 16,16 });
	m_player->GetCollider()->SetOffset({ 0,8 });
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

	//EnviroProps* tree = CreateEntity<EnviroProps>({200,200},{128,128}, "Oak_Tree", Shape::Rectangle);
	//tree->SetType(EnviroSpawnName::Tree);
	//tree->ShowCollider(true);
	//tree->SetRigidBody(true);
	//tree->SetTag(2);
	//tree->GetCollider()->Resize({ 16,16 });
	//tree->GetCollider()->SetOffset({ 0,28 });
	//tree->InitialisePhysicsBody(ShapeBodyPhysics::Static);
	//
	//GetCamera()->SetZoom(5.f);
	//GetCamera()->SetSmoothSpeed(2);
	//Application::Get()->SetClampCameraToCurrentMap(true);

	//MapTiled* map = mm->GetCurrentMap();

	//std::vector<TileInfo> canGrowTiles = map->GetAllTilesWithProperty("CanGrow", "CanGrow");
	//int size = canGrowTiles.size();
	//Vector2i tileDim = map->GetTileDimension();

	//for (const TileInfo& info : canGrowTiles)
	//{
	//	Vector2f center = {
	//		info.worldPos.GetX() + tileDim.GetX() / 2.f,
	//		info.worldPos.GetY() + tileDim.GetY() / 2.f
	//	};
	//	FarmTile* tile = CreateEntity<FarmTile>(center, tileDim, "", Shape::Rectangle, false);
	//	tile->GetCollider()->SetLayer(FARMTILE);
	//	tile->GetCollider()->SetMask(NoNE);
	//	//tile->ShowCollider(true);
	//	FarmManager::Get()->RegisterTile(tile);
	//}
	
	m_player2 = CreateEntity<Player>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 16,32 }, "player", Shape::Rectangle);
	m_player2->GetInventory()->AddItem("ITEM_WOOD", 30, false);
	m_player2->GetInventory()->AddItem("ITEM_FIBER", 30, false);

	UiShop* a = CreateEntity<UiShop>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 900, 600 }, "Perceval", Shape::Rectangle, true);
	a->ShowCollider(true);
	CreateText("time", { 200, 50 }, std::to_string(gm.GetHour()) + ":" + std::to_string(gm.GetMinute()), { 255, 255, 255, 255 }, true);

}


void GameScene::OnEvent()
{
	InputManager* im = InputManager::Get();
	if(im->IsMouseDown(BUTTON_RIGHT)) {
		EnviroSpawn* spawn = new EnviroSpawn();
		spawn->InitEnviroSpawn();
		spawn->SpawnEnviroProps();
	}

	if (im->IsKeyboardDown(K_a))
		GetCamera()->SetZoom(5);

	if (im->IsKeyboardDown(K_e))
		GetCamera()->SetZoom(1);
}

void GameScene::OnUpdate()
{
	//GetCamera()->Follow(m_player);
	//FarmManager::Get()->Update();
	GameData& gm = GameData::Get();
	gm.Update();
	CreateText("money", { 500, 10 }, std::to_string(m_player->GetMoney()), ColorsNamed::White, true);
	//Debug::Get()->GetText("time")->SetText(std::to_string(gm.GetHour()) + ":" + std::to_string(gm.GetMinute()));
	//std::cout << "X = " << m_Player->GetPosition().GetX() / 2 << " Y = " << m_Player->GetPosition().GetY() / 2 << std::endl;
}

void GameScene::OnDestroy()
{
}

void GameScene::OnSleep()
{
}

void GameScene::OnWake()
{
}
