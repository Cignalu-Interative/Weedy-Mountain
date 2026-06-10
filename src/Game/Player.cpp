#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

#include "FarmManager.h"   
#include "GameData.h" 

void Player::OnInitialize()
{
	PlayerJob* gatherer = new PlayerJob();
	gatherer->m_name = JobType::Gatherer;
	m_Jobs.push_back(gatherer);
	PlayerJob* farmer = new PlayerJob();
	farmer->m_name = JobType::Farmer;
	m_Jobs.push_back(farmer);
	PlayerJob* miner = new PlayerJob();
	miner->m_name = JobType::Miner;
	m_Jobs.push_back(miner);
	PlayerJob* killer = new PlayerJob();
	killer->m_name = JobType::Killer;
	m_Jobs.push_back(killer);
	PlayerJob* dealer = new PlayerJob();
	dealer->m_name = JobType::Dealer;
	m_Jobs.push_back(dealer);
}

void Player::OnEvent()
{
	if (InputManager::Get()->IsKeyboardHeld(K_LSHIFT)) {
		SetSpeed(m_Stats.m_PlayerSpeed * 2);
	}
	else SetSpeed(m_Stats.m_PlayerSpeed);

	//Input Clavier
	if (InputManager::Get()->IsKeyboardHeld(K_z)) {
		SetDirection({ GetDirection().GetX(), -1.0f });
		GetCollider("actionBox")->SetOffset({ 0, -8 });
		GetCollider("actionBox")->SyncWithOwner();
	}
	if (InputManager::Get()->IsKeyboardHeld(K_s)) {
		SetDirection({ GetDirection().GetX(), 1.0f });
		GetCollider("actionBox")->SetOffset({ 0, 24 });
		GetCollider("actionBox")->SyncWithOwner();
	}
	if (InputManager::Get()->IsKeyboardHeld(K_q)) {
		SetDirection({ -1.0f, GetDirection().GetY() });
		GetCollider("actionBox")->SetOffset({ -16, 8 });
		GetCollider("actionBox")->SyncWithOwner();
	}
	if (InputManager::Get()->IsKeyboardHeld(K_d)) {
		SetDirection({ 1.0f, GetDirection().GetY() });
		GetCollider("actionBox")->SetOffset({ 16, 8 });
		GetCollider("actionBox")->SyncWithOwner();
	}

	//input souris
	if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
		//utilise l'objet selectionner dans l'inventaire (Vivien a l'aide)
	}


	// TEMP - AGRI

	// Sélection plante 
	if (InputManager::Get()->IsKeyboardDown(K_1))
		m_selectedCrop = CropCatalog::Wheat();
	if (InputManager::Get()->IsKeyboardDown(K_2))
		m_selectedCrop = CropCatalog::Carrot();

	// Action farm 
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		HandleFarmAction();
}

// TEMP - AGRI
void Player::HandleFarmAction()
{
	Collider* actionBox = GetCollider("actionBox");

	if (!actionBox)
		return;

	FarmTile* target = FarmManager::Get()->GetTileInActionBox(actionBox);

	if (!target)
		return;

	int currentDay = GameData::Get().GetDay();

	switch (target->GetState())
	{
	case FarmTileState::IDLE:
		// Bêcher
		target->Till(currentDay);
		break;

	case FarmTileState::TILLED:
		// T maintenu = arroser, Y maintenu = planter, sinon = dé-bêcher
		if (InputManager::Get()->IsKeyboardHeld(K_t))
			target->Water(currentDay);
		else if (InputManager::Get()->IsKeyboardHeld(K_y))
			target->Plant(m_selectedCrop, currentDay);
		else
			target->Till(currentDay);
		break;

	case FarmTileState::WATERED:
		// Y maintenu = planter
		if (InputManager::Get()->IsKeyboardHeld(K_y))
			target->Plant(m_selectedCrop, currentDay);
		break;

	case FarmTileState::PLANTED:
	case FarmTileState::GROWING:
		// Arroser
		target->Water(currentDay);
		break;

	case FarmTileState::READY:
		// Récolter
		target->Harvest(currentDay);
		break;
	}
}

void Player::OnUpdate()
{
	for (int i = 0; i < m_Jobs.size(); i++) {
		m_Jobs[i]->levelUp();
	}
}

void Player::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnDestroy()
{

}