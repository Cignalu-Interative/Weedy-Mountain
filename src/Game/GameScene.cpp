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


void GameScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");

	m_Player = CreateEntity<Player>({ GetWindowWidth()/2.f, GetWindowHeight()/2.f}, {16,32}, "player", Shape::Rectangle);
	m_Player->GetCollider()->SetDimension({ 16,16 });
	m_Player->GetCollider()->SetOffset({ 0,8 });
	m_Player->SetRigidBody(true);
	m_Player->GetCollider()->SetID("playerCollider");
	m_Player->SetTag(1);
	m_Player->SetDrawLayer(2);
	m_Player->AddCollider("actionBox", { 16,16 }, Shape::Rectangle, { 0,0 }, true);
	m_Player->AddCollider("interact", { 80,80 }, Shape::Circle, { 0,0 }, true);
	m_Player->ShowCollider(true);
	EnviroProps* Tree = CreateEntity<EnviroProps>({200,200},{128,128}, "Oak_Tree", Shape::Rectangle);
	Tree->SetType(EnviroSpawnName::Tree);
	Tree->ShowCollider(true);
	Tree->SetRigidBody(true);
	Tree->SetTag(2);
	Tree->GetCollider()->Resize({ 16,16 });
	Tree->GetCollider()->SetOffset({ 0,28 });
	
	GetCamera()->SetZoom(1.75f);
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
