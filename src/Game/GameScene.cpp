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

void GameScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	MapTiledManager* mm = MapTiledManager::Get();
	mm->AddMaps("../../res/Game/Assets/Maps/Maps_Repertory.json");
	mm->SetCurrentMap("Farm");

	GameData& gm = GameData::Get();
	gm.SetTimeScale(100);
	gm.Initialize();

	m_Player = CreateEntity<Player>({570, 100}, {16,32}, "player", Shape::Rectangle);
	m_Player->GetCollider()->SetDimension({ 16,16 });
	m_Player->GetCollider()->SetOffset({ 0,8 });
	m_Player->SetRigidBody(true);
	m_Player->GetCollider()->SetID("playerCollider");
	m_Player->SetTag(1);
	m_Player->SetDrawLayer(2);
	m_Player->GetCollider("playerCollider")->SetLayer(1);
	m_Player->AddCollider("actionBox", { 16,16 }, Shape::Rectangle, { 0,0 }, true)->SetLayer(1);
	m_Player->AddCollider("interact", { 80,80 }, Shape::Circle, { 0,0 }, true)->SetLayer(1);
	m_Player->ShowCollider(true);
	EnviroProps* Tree = CreateEntity<EnviroProps>({200,200},{128,128}, "Oak_Tree", Shape::Rectangle);
	Tree->SetType(EnviroSpawnName::Tree);
	Tree->ShowCollider(true);
	Tree->SetRigidBody(true);
	Tree->SetIsStatic(true);
	Tree->SetTag(2);
	Tree->GetCollider()->Resize({ 16,16 });
	Tree->GetCollider()->SetOffset({ 0,28 });
	
	GetCamera()->SetZoom(5.f);
	GetCamera()->Follow(m_Player, GetDeltaTime());
	Application::Get()->SetClampCameraToCurrentMap(true);

	MapTiled* map = mm->GetCurrentMap();

	// TUE LES FPS (marchais avant le merge dans la branche P1-Agriculture)

	/*
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
		tile->SetDrawLayer(0);
		tile->GetCollider()->SetLayer(30);
		tile->GetCollider()->SetMask(1);
		//tile->ShowCollider(true);
		FarmManager::Get()->RegisterTile(tile);
	}
	*/

	CreateText("time", { 200, 50 }, std::to_string(gm.GetHour()) + ":" + std::to_string(gm.GetMinute()), { 255, 255, 255, 255 }, true);
}

void GameScene::OnEvent()
{
	if(InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
		EnviroSpawn* spawn = new EnviroSpawn();
		spawn->InitEnviroSpawn();
		spawn->SpawnEnviroProps();
	}
}

void GameScene::OnUpdate()
{
	GetCamera()->SetPosition(m_Player->GetPosition());
	FarmManager::Get()->Update();
	GameData& gm = GameData::Get();
	gm.Update();
	Debug::Get()->GetText("time")->SetText(std::to_string(gm.GetHour()) + ":" + std::to_string(gm.GetMinute()));
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
