#include "pch.h"
#include "CaveScene.h"
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

#include "PNJ.h"
#include "Ui/UiShop.h"
#include "SpriteContainer.h"

#include "GameScene.h"
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


void CaveScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	MapTiledManager* mm = MapTiledManager::Get();
	mm->AddMaps("../../res/Game/Assets/Maps/Maps_Repertory.json");
	mm->SetCurrentMap("Cave");

	m_player = CreateEntity<Player>({0,0}, {16,32}, "player", Shape::Rectangle);
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

}

void CaveScene::OnEvent()
{
	InputManager* im = InputManager::Get();

	if (im->IsKeyboardDown(K_C))
	{
		SceneManager::Get()->PushScene("game");
	}
}

void CaveScene::OnUpdate()
{
}

void CaveScene::OnDestroy()
{
}

void CaveScene::OnSleep()
{
	SetSleepDraw(true);
	SetSleepEvent(true);
	m_player->SetIsVisible(false);
}

void CaveScene::OnWake()
{
	SetSleepDraw(false);
	SetSleepEvent(false);
	m_player->SetIsVisible(true);

	MapTiledManager* mm = MapTiledManager::Get();
	mm->SetCurrentMap("Cave");

	GameData::Get().SetCurrentScene(CurrentScene::CAVE);
	GetCamera()->SetPosition(m_player->GetPosition());
	GetCamera()->SetFollowedEntity(m_player);
}

void CaveScene::OnDraw()
{
}
