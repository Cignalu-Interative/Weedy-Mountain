#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

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
	}
	if (InputManager::Get()->IsKeyboardHeld(K_s)) {
		SetDirection({ GetDirection().GetX(), 1.0f });
		GetCollider("actionBox")->SetOffset({ 0, 24 });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_q)) {
		SetDirection({ -1.0f, GetDirection().GetY() });
		GetCollider("actionBox")->SetOffset({ -16, 8 });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_d)) {
		SetDirection({ 1.0f, GetDirection().GetY() });
		GetCollider("actionBox")->SetOffset({ 16, 8 });
	}


	//input souris
	if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
		//utilise l'objet selectionner dans l'inventaire (Vivien a l'aide)
	}
}

void Player::OnUpdate()
{
	UpdateJobs();
}

void Player::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnDestroy()
{

}

void Player::UpdateJobs()
{
	for (int i = 0; i < m_Jobs.size(); i++) {
		m_Jobs[i]->levelUp();
		switch (m_Jobs[i]->m_name)
		{
		case JobType::Gatherer:
			switch (m_Jobs[i]->m_lvl) {
			case 2:
				//unlockCraft(Bandage L1);
				break;
			case 3:
				//unlockCraft(Energizing remedy L1);
				break;
			case 4:
				GetStats().m_harvestEffiency *= 1.067f;
				break;
			case 5:
				//unlockCraft(Bandage L2);
				break;
			case 6:
				//unlockCraft(Energizing remedy L2);
				break;
			}break;

		case JobType::Farmer:
			switch (m_Jobs[i]->m_lvl) {
			case 2:
				//unlockCraft(fertilizer L1);
				break;
			case 3:
				//unlockCraft(autoWatering L1);
				break;
			case 4:
				GetStats().m_farmerSpeed *= 1.067f;
				break;
			case 5:
				//unlockCraft(fertilizer L2);
				break;
			case 6:
				//unlockCraft(autoWatering L2);
				break;
			}break;
			break;

		case JobType::Miner:
			switch (m_Jobs[i]->m_lvl) {
			case 2:
				//unlockCraft(stone_tools);
				break;
			case 3:
				//unlockCraft(iron_tools);
				break;
			case 4:
				GetStats().m_miningSpeed *= 1.067f;
				break;
			case 5:
				//unlockCraft(gold_tools);
				break;
			case 6:
				//unlockCraft(diamond_tools);
				break;
			}break;
			break;


		case JobType::Dealer:
			switch (m_Jobs[i]->m_lvl) {
			case 2:
				GetStats().m_drugsSellQuantity += 15;
				break;
			case 3:
				GetStats().m_canBuyWeedSeed = true;
				break;
			case 4:
				GetStats().m_drugSellEfficency *= 1.067f;
				break;
			case 5:
				GetStats().m_drugsSellQuantity += 40;
				break;
			case 6:
				GetStats().m_canBuyCannabis = true;
				break;
			}break;
			break;
		case JobType::Killer:
			switch (m_Jobs[i]->m_lvl) {
			case 2:
				GetStats().m_canBuyKnife = true;
				break;
			case 3:
				GetStats().m_canBuySword = true;
				break;
			case 4:
				GetStats().m_PlayerSpeed *= 1.067f;
				break;
			case 5:
				GetStats().m_canBuyGlock = true;
				break;
			case 6:
				GetStats().m_canBuyRifle = true;
				break;
			}break;
			break;
		}
	}
}
