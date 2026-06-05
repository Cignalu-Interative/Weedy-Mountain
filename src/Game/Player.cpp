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
	if (InputManager::Get()->IsKeyboardHeld(K_z) && !m_Walls[0]) {
		SetDirection({ GetDirection().GetX(), -1.0f });
		GetCollider("actionBox")->SetOffset({ 0, -8 });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_s) && !m_Walls[1]) {
		SetDirection({ GetDirection().GetX(), 1.0f });
		GetCollider("actionBox")->SetOffset({ 0, 24 });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_q) && !m_Walls[2]) {
		SetDirection({ -1.0f, GetDirection().GetY() });
		GetCollider("actionBox")->SetOffset({ -16, 8 });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_d) && !m_Walls[3]) {
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

void Player::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 2 && _self->GetID() == "playerCollider") {
		switch (_side)
		{
		case CollisionSide::TOP:
			m_Walls[0] = true;
			break;
		case CollisionSide::BOTTOM:
			m_Walls[1] = true;
			break;
		case CollisionSide::LEFT:
			m_Walls[2] = true;
			break;
		case CollisionSide::RIGHT:
			m_Walls[3] = true;
			break;
		default:
			break;
		}
	}
}

void Player::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 2 && _self->GetID() == "playerCollider") {
		if (_side == CollisionSide::NONE) {
			for (int i = 0; i < m_Walls.size(); i++) {
				m_Walls[i] = false;
			}
		}
	}
}

void Player::OnDestroy()
{

}