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
	for (int i = 0; i < m_Jobs.size(); i++) {
		m_Jobs[i]->levelUp();
	}
}

void Player::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnDestroy()
{

}

void Player::GetJobsRewards()
{
	for (int i = 0; i < m_Jobs.size(); i++) {
		
		switch (m_Jobs[i]->m_name)
		{
		case JobType::Gatherer:
			switch (m_Jobs[i]->m_lvl)
			{
				case 1
			}
			break;
		case JobType::Farmer:
			break;
		case JobType::Dealer:
			break;
		case JobType::Miner:
			break;
		case JobType::Killer:
			break;
		}


	}
}
