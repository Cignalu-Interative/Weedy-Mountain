#pragma once
#include "StateBase.h"
#include "Policeman.h"

class PolicemanIdleState : public StateBase<Policeman>
{
private:

	float m_cooldown = 0.f;

public:

	void Start(Policeman* p_policeman) {
		p_policeman->SetAnimationByName("Idle");
		std::cout << "Idle" << std::endl;

		m_cooldown = 3.f;
		p_policeman->SetIdleCooldownEnded(false);
	};

	void Update(Policeman* p_policeman, float dt) 
	{
		m_cooldown -= p_policeman->GetDeltaTime();

		if (m_cooldown <= 0.f)
		{
			p_policeman->SetIdleCooldownEnded(true);
		}
	};

	void End(Policeman* p_policeman) {};
	bool CanTransitionFrom(Policeman* p_policeman, int currentState)
	{
		switch (currentState)
		{
		case (int)PolicemanState::PATROL:
			return true;
			break;
		case (int)PolicemanState::CHASE:
			return true;
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
			return false;
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