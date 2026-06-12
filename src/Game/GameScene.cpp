#include "pch.h"
#include "GameScene.h"
#include "EnviroProps.h"
#include "EnviroSpawn.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

#include "SpriteContainer.h"

#include "DialogueManager.h"

void GameScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	DialogueManager::Get()->AddDialogueByPath("../../res/2DPendentEngine/Assets/Dialogues/Dialogue/test.txt");

	m_Player = CreateEntity<Player>({ 370, 200 }, { 16,32 }, "player", Shape::Rectangle);
	m_Player->GetCollider()->SetDimension({ 16,16 });
	m_Player->GetCollider()->SetOffset({ 0,8 });
	m_Player->SetRigidBody(true);
	m_Player->GetCollider()->SetID("playerCollider");
	m_Player->SetTag(1);
	m_Player->SetDrawLayer(2);
	m_Player->AddCollider("actionBox", { 16,16 }, Shape::Rectangle, { 0,0 }, true);
	m_Player->AddCollider("interact", { 80,80 }, Shape::Circle, { 0,0 }, true);
	m_Player->ShowCollider(true);


	m_pnj = CreateEntity<PNJ>({ 350,200 }, { 16,32 }, "player", Shape::Rectangle);
	m_pnj->GetCollider()->SetDimension({ 16,16 });
	m_pnj->GetCollider()->SetOffset({ 0,8 });
	m_pnj->SetRigidBody(true);
	m_pnj->ShowCollider(true);
	m_pnj->SetDialogPath("test");

	EnviroProps* Tree = CreateEntity<EnviroProps>({ 200,200 }, { 128,128 }, "Oak_Tree", Shape::Rectangle);
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
	if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
		EnviroSpawn* spawn = new EnviroSpawn();
		spawn->InitEnviroSpawn();
		spawn->SpawnEnviroProps();
	}

	if (InputManager::Get()->IsKeyboardDown(K_e) && m_pnj->IsInPlayerZone() && !m_pnj->IsTalking())
	{
		m_pnj->Talk(1, 2);

		

	}
}

void GameScene::OnUpdate()
{
	GetCamera()->SetPosition(m_Player->GetPosition());
	if (m_pnj->IsTalking())
		m_Player->SetIsInDialog(true);
	else
		m_Player->SetIsInDialog(false);

	if (m_pnj->IsFinished() && m_pnj->IsTalking())
	{
		for (Dialogue::Choice choice : m_pnj->GetChoices())
		{
			static int i = 1;
			std::string id = choice.s_label + std::to_string(i++);
			Button* button = CreateButton({ 250.f * i, 200 }, { 50, 25 }, "", id, choice.s_label, true);
			button->SetOnClick([this, id, choice] {
				m_pnj->Talk(choice.s_target, 2);

				});
			button->ShowCollider(true);
			button->SyncColliders();
		}

	}
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
