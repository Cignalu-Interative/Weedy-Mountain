#include "pch.h"
#include "GameScene.h"
#include "EnviroProps.h"
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

	m_Player = CreateEntity<Player>({ GetWindowWidth()/2.f, GetWindowHeight()/2.f}, {32,64}, "Player", Shape::Rectangle);
	m_Player->ShowCollider(true);
	EnviroProps* Tree = CreateButton<EnviroProps>({200,200},{128,160}, "Oak_Tree", "", "", false);
	Tree->SetType(EnviroName::Tree);
	Tree->ShowCollider(true);
}

void GameScene::OnEvent()
{
}

void GameScene::OnUpdate()
{
	GetCamera()->SetPosition(m_Player->GetPosition());
}

void GameScene::OnDestroy()
{
}
