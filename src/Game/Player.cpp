#include "pch.h"
#include "Player.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

void Player::OnInitialize()
{
	Job* gatherer = new Job();
	m_Jobs.push_back(gatherer);
	Job* farmer = new Job();
	m_Jobs.push_back(farmer);
	Job* miner = new Job();
	m_Jobs.push_back(miner);
	Job* killer = new Job();
	m_Jobs.push_back(killer);
	Job* dealer = new Job();
	m_Jobs.push_back(dealer);
}

void Player::OnEvent()
{
	if (InputManager::Get()->IsKeyboardHeld(K_LSHIFT)) {
		SetSpeed(m_PlayerSpeed * 2);
	}
	else SetSpeed(m_PlayerSpeed);

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
}

void Player::OnCollision(Entity* other)
{

}

void Player::OnDestroy()
{

}