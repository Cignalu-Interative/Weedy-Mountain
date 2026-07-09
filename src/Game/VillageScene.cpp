#include "pch.h"
#include "VillageScene.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/FontManager.h"

#include "Player.h"
#include "GameData.h"

#include "GameScene.h"
#include "Ui/UiStats.h"
#include "Ui/UiMoney.h"
#include "Ui/UiCash.h"
#include "Ui/UiDate.h"
#include "Ui/UiTime.h"
#include "Ui/UiJobs.h"
#include "Ui/UiGatherer.h"

#include "2DPendentLib/ObjectGroupTiled.h"
#include "2DPendentLib/ObjectTiled.h"

void VillageScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	MapTiledManager* mm = MapTiledManager::Get();
	mm->AddMaps("../../res/Game/Assets/Maps/Maps_Repertory.json");
	mm->SetCurrentMap("Village");

	ObjectGroupTiled* a = mm->GetCurrentMap()->GetObjectGroupByName("Object");
	std::vector<ObjectTiled*> h = a->GetAllObjects();

	for (ObjectTiled* o : h)
	{
		if (o->GetObjectName() == "PlayerSpawn")
		{
			m_spawnPoint = o->GetObjectPosition();
			break;
		}
	}

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

	
	GetCamera()->SetFollowedEntity(m_player);
	GetCamera()->SetPosition(m_spawnPoint);
	Application::Get()->SetClampCameraToCurrentMap(true);

}

void VillageScene::OnEvent()
{
	InputManager* im = InputManager::Get();

	if (im->IsKeyboardDown(K_X))
	{
		SceneManager::Get()->PushScene("game");
	}
	if (im->IsKeyboardDown(K_M))
		m_player->TakeDamage(5);
	if (im->IsKeyboardDown(K_U))
		m_player->Heal(5);

}

void VillageScene::OnUpdate()
{
}

void VillageScene::OnDestroy()
{
}

void VillageScene::OnSleep()
{
}

void VillageScene::OnWake()
{
	MapTiledManager* mm = MapTiledManager::Get();
	mm->SetCurrentMap("Village");
}

void VillageScene::OnDraw()
{
}
