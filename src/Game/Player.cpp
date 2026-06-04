#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

void Player::OnInitialize()
{
	m_interactZone = GetScene()->CreateEntity<User>(GetPosition(), { 160, 160 }, "Oak_Tree", Shape::Circle);
	m_interactZone->ShowCollider(true);

	PlayerJob* gatherer = new PlayerJob();
	gatherer->name = "Gatherer";
	m_Jobs.push_back(gatherer);
	PlayerJob* farmer = new PlayerJob();
	farmer->name = "Farmer";
	m_Jobs.push_back(farmer);
	PlayerJob* miner = new PlayerJob();
	miner->name = "Miner";
	m_Jobs.push_back(miner);
	PlayerJob* killer = new PlayerJob();
	killer->name = "Killer";
	m_Jobs.push_back(killer);
	PlayerJob* dealer = new PlayerJob();
	dealer->name = "Dealer";
	m_Jobs.push_back(dealer);
}

void Player::OnEvent()
{
	if (InputManager::Get()->IsKeyboardHeld(K_LSHIFT)) {
		SetSpeed(m_Stats.m_PlayerSpeed * 2);
	}
	else SetSpeed(m_Stats.m_PlayerSpeed);

	if (InputManager::Get()->IsKeyboardHeld(K_z)) {
		SetDirection({ GetDirection().GetX(), -1.0f });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_s)) {
		SetDirection({ GetDirection().GetX(), 1.0f });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_q)) {
		SetDirection({ -1.0f, GetDirection().GetY() });
	}
	if (InputManager::Get()->IsKeyboardHeld(K_d)) {
		SetDirection({ 1.0f, GetDirection().GetY() });
	}
}

void Player::OnUpdate()
{
	for (int i = 0; i < m_Jobs.size(); i++) {
		m_Jobs[i]->levelUp();
	}

	m_interactZone->SetPosition(GetPosition());
}

void Player::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnDestroy()
{
	m_interactZone->SetToDestroy(true);
}