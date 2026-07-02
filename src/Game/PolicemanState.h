#pragma once
#include "StateBase.h"
#include "Policeman.h"

class PolicemanIdleState : public StateBase<Policeman>
{
	void Start(Policeman* p_policeman) {
		p_policeman->SetAnimationByName("Idle");
		std::cout << "Idle" << std::endl;
	};

	void Update(Policeman* p_player, float dt) {};
	void End(Policeman* p_player) {};
	bool CanTransitionFrom(Policeman* p_player, int currentState)
	{
		switch (currentState)
		{
		case (int)PolicemanState::PATROL:
			return true;
			break;
		case (int)PolicemanState::CHASE:
			return false;
			break;
		}
	};
};

class PolicemanPatrolState : public StateBase<Policeman>
{
	void Start(Policeman* p_policeman) 
	{
		std::cout << "Patrol" << std::endl;
	};

	void Update(Policeman* p_policeman, float dt)
	{
		Vector2f policemanDir = p_policeman->GetDirection();
		if (policemanDir.GetX() > 0)
			p_policeman->SetAnimationByName("WalkRight");
		else if (policemanDir.GetX() < 0)
			p_policeman->SetAnimationByName("WalkLeft");

		if (policemanDir.GetY() > 0)
			p_policeman->SetAnimationByName("WalkDown");
		else if (policemanDir.GetY() < 0)
			p_policeman->SetAnimationByName("WalkUp");

	};
	void End(Policeman* p_policeman) {};

	bool CanTransitionFrom(Policeman* p_policeman, int currentState)
	{
		switch (currentState)
		{
		case (int)PolicemanState::IDLE:
			return true;
			break;
		case (int)PolicemanState::CHASE:
			return true;
			break;
		}
	};
};

class PolicemanChaseState : public StateBase<Policeman>
{
	void Start(Policeman* p_policeman) {
		std::cout << "Chase" << std::endl;
	};

	void Update(Policeman* p_policeman, float dt) 
	{
		Vector2f policemanDir = p_policeman->GetDirection();
		if (policemanDir.GetX() > 0)
			p_policeman->SetAnimationByName("WalkRight");
		else if (policemanDir.GetX() < 0)
			p_policeman->SetAnimationByName("WalkLeft");

		if (policemanDir.GetY() > 0)
			p_policeman->SetAnimationByName("WalkDown");
		else if (policemanDir.GetY() < 0)
			p_policeman->SetAnimationByName("WalkUp");

	};
	void End(Policeman* p_policeman) {};

	bool CanTransitionFrom(Policeman* p_policeman, int currentState)
	{
		switch (currentState)
		{
		case (int)PolicemanState::IDLE:
			return true;
			break;
		case (int)PolicemanState::PATROL:
			return false;
			break;
		}
	};
};