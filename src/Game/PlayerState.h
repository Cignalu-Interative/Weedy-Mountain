#pragma once
#include "StateBase.h"
#include "Player.h"

class PlayerIdleState : public StateBase<Player>
{
	void Start(Player* p_player) override {
		std::cout << "Idle" << std::endl;
		p_player->SetAnimationByName("Idle");
	};

	void Update(Player* p_player, float dt)override 
	{
		
	};
	void End(Player* p_player) override {};
	bool CanTransitionFrom(Player* p_player, int currentState)override
	{
		return true;
	};
};

class PlayerWalkingState : public StateBase<Player>
{
	void Start(Player* p_player) override {
		std::cout << "Walking" << std::endl;
	};

	void Update(Player* p_player, float dt) override
	{
		Vector2f playerDir = p_player->GetDirection();
		if (playerDir.GetX() > 0)
			p_player->SetAnimationByName("WalkRight");
		else if (playerDir.GetX() < 0)
			p_player->SetAnimationByName("WalkLeft");

		if (playerDir.GetY() > 0)
			p_player->SetAnimationByName("WalkDown");
		else if (playerDir.GetY() < 0)
			p_player->SetAnimationByName("WalkUp");

		
	};

	void End(Player* p_player)override {};

	bool CanTransitionFrom(Player* p_player, int currentState)override
	{
		return true;
	};
};

class PlayerWateringState : public StateBase<Player>
{
	void Start(Player* p_player) override {
		p_player->SetAnimationByName("Watering");
		std::cout << "watering" << std::endl;
	};

	void Update(Player* p_player, float dt) override {};
	void End(Player* p_player) override {};

	bool CanTransitionFrom(Player* p_player, int currentState)override
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::DIGGING:
			return false;
			break;
		case (int)PlayerState::WALKING:
			return false;
			break;
		}
	};
};

class PlayerDiggingState : public StateBase<Player>
{
	void Start(Player* p_player)override {
		p_player->SetAnimationByName("Digging");
		std::cout << "digging" << std::endl;
	};

	void Update(Player* p_player, float dt) override {};
	void End(Player* p_player) override {};

	bool CanTransitionFrom(Player* p_player, int currentState)override
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		case (int)PlayerState::WALKING:
			return false;
			break;
		}
	};
};

class PlayerMiningState : public StateBase<Player>
{
	void Start(Player* p_player) override {
		p_player->SetAnimationByName("Mining");
		std::cout << "mining" << std::endl;
	};

	void Update(Player* p_player, float dt) override {};
	void End(Player* p_player) override {};

	bool CanTransitionFrom(Player* p_player, int currentState)override
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		case (int)PlayerState::DIGGING:
			return false;
			break;
		case (int)PlayerState::WALKING:
			return false;
			break;
		}
	};
};