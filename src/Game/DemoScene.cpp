#include "pch.h"
#include "DemoScene.h"
#include "Player.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

inline Debug& debug = *Debug::Get();
inline InputManager& input = *InputManager::Get();
inline SoundManager& sound = *SoundManager::Get();

DemoScene::DemoScene()
{
	m_players.reserve(64); // Réserver de la mémoire pour 64 entites afin d'éviter les reallocations fréquentes lors de l'ajout de joueurs
}

void DemoScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/2DPendentEngine/Assets/Textures/Textures_Repertory.json");
	MapTiledManager::Get()->AddMaps("../../res/2DPendentEngine/Assets/Maps/Maps_Repertory.json");
	SoundManager::Get()->AddSound("../../res/2DPendentEngine/Assets/Sounds/Sounds_Repertory.json");
	FontManager::Get()->AddFontByRepertory("../../res/2DPendentEngine/Assets/Fonts/Fonts_Repertory.json");


	Player* player1 = CreateEntity<Player>({ 200, 0 }, { 200, 200 }, "Perceval", Shape::Rectangle);
	player1->SetAngle(0.0f);
	player1->InitialisePhysicsBody(ShapeBodyPhysics::Dynamic);
	player1->RemoveCollider("default");
	player1->AddCollider("default", { 200, 200 }, Shape::Rectangle, { 0, 0 });
	player1->ShowCollider(true);
	m_player1 = player1;
	m_players.push_back(player1);

	Player* player2 = CreateEntity<Player>({ 200, 400 }, { 500, 100 }, "Perceval", Shape::Rectangle);
	player2->SetAngle(0.0f);
	player2->InitialisePhysicsBody(ShapeBodyPhysics::Static);
	player2->ShowCollider(true);
	m_player2 = player2;
	m_players.push_back(player2);
}

void DemoScene::OnEvent()
{
	
}

void DemoScene::OnUpdate()
{
	std::cout << "Player1 = " <<m_player1->GetIsGrounded() << std::endl;
	std::cout << "Player2 = " <<m_player2->GetIsGrounded() << std::endl;
}

void DemoScene::OnSleep()
{
	SetIsVisible(false);
	SetSleepEvent(true);
	SetSleepUpdate(true);
}

void DemoScene::OnWake()
{
	SetIsVisible(true);
	SetSleepEvent(false);
	SetSleepUpdate(false);
}

void DemoScene::OnDestroy()
{
	m_players.clear();
	debug.ClearAll();
	GetEntities().clear();
}

void DemoScene::TestVélocité()
{
	for (Player* player : m_players)
	{
		if (player->GetTag() == 64 && player->GetPhysicsBody())
		{
			Vector2f vel = player->GetPhysicsBody()->GetLinearVelocity();
			std::cout << "Velocity : X=" << vel.GetX()
				<< " Y=" << vel.GetY() << std::endl;
		}
	}
}

void DemoScene::SelectEntity(Vector2f p_position)
{
	RectangleCollider* collider = new RectangleCollider();
	collider->Initialize(p_position, { 10, 10 });
	for (Player* player : m_players)
	{
		if (Collision::IsColliding(*collider, *player->GetCollider()))
		{
			if (entitySelected != nullptr)
			{
				lastEntitySelected = entitySelected;
			}
			entitySelected = player;
			return;
		}
	}
}