#include "pch.h"
#include "DemoScene.h"
#include "Player.h"
#include "User.h"
#include "Inventory.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

#include <iostream>

inline Debug& debug = *Debug::Get();
inline InputManager& input = *InputManager::Get();
inline SoundManager& sound = *SoundManager::Get();

DemoScene::DemoScene()
{
	m_players.reserve(64); // Réserver de la mémoire pour 64 entites afin d'éviter les reallocations fréquentes lors de l'ajout de joueurs
}

void DemoScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	MapTiledManager::Get()->AddMaps("../../res/2DPendentEngine/Assets/Maps/Maps_Repertory.json");
	SoundManager::Get()->AddSound("../../res/2DPendentEngine/Assets/Sounds/Sounds_Repertory.json");
	FontManager::Get()->AddFontByRepertory("../../res/2DPendentEngine/Assets/Fonts/Fonts_Repertory.json");

	m_inv = CreateEntity<Inventory>({ GetWindowWidth() / 2.0f, GetWindowHeight() / 2.0f}, {636, 306}, "InventoryWeedyMountain", Shape::Rectangle, true);
}

void DemoScene::OnEvent()
{
	if (input.IsKeyboardDown(K_a))
	{
		m_inv->AddItem(ItemID::ITEM_EGG, 32, false);
		std::cout << "adddedd" << std::endl;
	}
	if (input.IsKeyboardDown(K_z))
	{
		m_inv->AddItem(ItemID::ITEM_POTATOE, 32, false);
		std::cout << "adddedd" << std::endl;
	}
	if (input.IsKeyboardDown(K_e))
	{
		m_inv->AddItem(ItemID::ITEM_SUN_FLOWER, 32, false);
		std::cout << "adddedd" << std::endl;
	}
	if (input.IsKeyboardDown(K_r))
	{
		m_inv->AddItem(ItemID::ITEM_HELMET, 3, false);
		std::cout << "adddedd" << std::endl;
	}
	if (input.IsKeyboardDown(K_t))
	{
		m_inv->AddItem(ItemID::ITEM_FISHING_ROD, 3, false);
		std::cout << "adddedd" << std::endl;
	}


	if (input.IsKeyboardDown(K_q))
	{
		m_inv->RemoveItem(ItemID::ITEM_EGG, 32, false);
		std::cout << "rmv" << std::endl;
	}
	if (input.IsKeyboardDown(K_s))
	{
		m_inv->RemoveItem(ItemID::ITEM_POTATOE, 32, false);
		std::cout << "rmv" << std::endl;
	}
	if (input.IsKeyboardDown(K_d))
	{
		m_inv->RemoveItem(ItemID::ITEM_SUN_FLOWER, 32, false);
		std::cout << "rmv" << std::endl;
	}
	if (input.IsKeyboardDown(K_f))
	{
		m_inv->RemoveItem(ItemID::ITEM_HELMET, 3, false);
		std::cout << "rmv" << std::endl;
	}
	if (input.IsKeyboardDown(K_g))
	{
		m_inv->RemoveItem(ItemID::ITEM_FISHING_ROD, 3, false);
		std::cout << "rmv" << std::endl;
	}
}

void DemoScene::OnUpdate()
{
	std::cout << m_inv->GetSlot(0)->GetStack().GetAmount() << std::endl;
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
	/*for (Player* player : m_players)
	{
		player->Destroy();
	}
	if (m_user != nullptr)
	{
		m_user->Destroy();
		m_user = nullptr;
	}
	if (entitySelected != nullptr)
	{
		entitySelected = nullptr;
	}
	if (lastEntitySelected != nullptr)
	{
		lastEntitySelected = nullptr;
	}*/
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