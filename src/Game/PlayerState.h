#pragma once
#include "StateBase.h"
#include "Player.h"

class PlayerIdleState : public StateBase<Player>
{
	void Start(Player* p_player) {
		//p_player->SetAnimationByName("Idle");
		std::cout << "Idle" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) {};
	bool CanTransitionFrom(Player* p_player, int currentState) 
	{
		switch (currentState)
		{
		case (int)PlayerState::WALKUP : 
			return true;
			break;
		case (int)PlayerState::WALKDOWN : 
			return true;
			break;
		case (int)PlayerState::WALKRIGHT:
			return true;
			break;
		case (int)PlayerState::WALKLEFT:
			return true;
			break;
		case (int)PlayerState::MINING:
			return true;
			break;
		case (int)PlayerState::DIGGING:
			return true;
			break;
		case (int)PlayerState::WATERING:
			return true;
			break;
		
		}
	};
};

class PlayerWalkUpState : public StateBase<Player>
{
	void Start(Player* p_player) {
	//	p_player->SetAnimationByName("WalkUp");
		std::cout << "walkup" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) { };
	bool CanTransitionFrom(Player* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WALKDOWN:
			return true;
			break;
		case (int)PlayerState::WALKRIGHT:
			return true;
			break;
		case (int)PlayerState::WALKLEFT:
			return true;
			break;
		case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::DIGGING:
			return false;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		}
	};
};

class PlayerWalkDownState : public StateBase<Player>
{
	void Start(Player* p_player) {
		//p_player->SetAnimationByName("WalkDown");
		std::cout << "walkDown" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) {};
	bool CanTransitionFrom(Player* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WALKUP:
			return true;
			break;
		case (int)PlayerState::WALKRIGHT:
			return true;
			break;
		case (int)PlayerState::WALKLEFT:
			return true;
			break;
			case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::DIGGING:
			return false;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		}
	};
};

class PlayerRightState : public StateBase<Player>
{
	void Start(Player* p_player) {
		//p_player->SetAnimationByName("WalkRight");
		std::cout << "walkright" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) {};
	bool CanTransitionFrom(Player* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WALKUP:
			return true;
			break;
		case (int)PlayerState::WALKRIGHT:
			return true;
			break;
		case (int)PlayerState::WALKLEFT:
			return true;
			break;
		case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::DIGGING:
			return false;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		}
	};
};

class PlayerLeftState : public StateBase<Player>
{
	void Start(Player* p_player) {
		//p_player->SetAnimationByName("WalkLeft");
		std::cout << "walkleft" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) {};

	bool CanTransitionFrom(Player* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WALKUP:
			return true;
			break;
		case (int)PlayerState::WALKRIGHT:
			return true;
			break;
		case (int)PlayerState::WALKLEFT:
			return true;
			break;
		case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::DIGGING:
			return false;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		}
	};
};

class PlayerWateringState : public StateBase<Player>
{
	void Start(Player* p_player) {
		//p_player->SetAnimationByName("WalkLeft");
		std::cout << "watering" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) {};

	bool CanTransitionFrom(Player* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WALKUP:
			return false;
			break;
		case (int)PlayerState::WALKRIGHT:
			return false;
			break;
		case (int)PlayerState::WALKLEFT:
			return false;
			break;
		case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::DIGGING : 
			return false;
			break;
		}
	};
};

class PlayerDiggingState : public StateBase<Player>
{
	void Start(Player* p_player) {
		//p_player->SetAnimationByName("WalkLeft");
		std::cout << "digging" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) {};

	bool CanTransitionFrom(Player* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WALKUP:
			return false;
			break;
		case (int)PlayerState::WALKRIGHT:
			return false;
			break;
		case (int)PlayerState::WALKLEFT:
			return false;
			break;
		case (int)PlayerState::MINING:
			return false;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		}
	};
};

class PlayerMiningState : public StateBase<Player>
{
	void Start(Player* p_player) {
		//p_player->SetAnimationByName("WalkLeft");
		std::cout << "mining" << std::endl;
	};
	
	void Update(Player* p_player, float dt) {};
	void End(Player* p_player) {};

	bool CanTransitionFrom(Player* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PlayerState::IDLE:
			return true;
			break;
		case (int)PlayerState::WALKUP:
			return false;
			break;
		case (int)PlayerState::WALKRIGHT:
			return false;
			break;
		case (int)PlayerState::WALKLEFT:
			return false;
			break;
		case (int)PlayerState::WATERING:
			return false;
			break;
		case (int)PlayerState::DIGGING:
			return false;
			break;
		}
	};
};