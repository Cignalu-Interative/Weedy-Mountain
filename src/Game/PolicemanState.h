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

		m_cooldown = 1.5f;
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
		p_policeman->StartPatrol();
	};

	void Update(Policeman* p_policeman, float dt)
	{
		/*Vector2f policemanDir = p_policeman->GetDirection();
		if (policemanDir.GetX() > 0)
			p_policeman->SetAnimationByName("WalkRight");
		else if (policemanDir.GetX() < 0)
			p_policeman->SetAnimationByName("WalkLeft");

		if (policemanDir.GetY() > 0)
			p_policeman->SetAnimationByName("WalkDown");
		else if (policemanDir.GetY() < 0)
			p_policeman->SetAnimationByName("WalkUp");*/

		int pathIndex = p_policeman->GetPathIndex();
		std::vector<Tile*> path = p_policeman->GetPath();

		Vector2i policemanGridPos = { (int)p_policeman->GetPosition().GetX() / 16, (int)p_policeman->GetPosition().GetY() / 16 };

		Vector2f dir = { path[pathIndex]->gridPos.GetX() - policemanGridPos.GetX(),
			path[pathIndex]->gridPos.GetY() - policemanGridPos.GetY() };

		p_policeman->SetDirection(dir);

		if (policemanGridPos == path[pathIndex]->gridPos)
		{
			p_policeman->SetPathIndex(pathIndex++);
		}
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
		p_policeman->StartChase();
	};

	void Update(Policeman* p_policeman, float dt) 
	{
		/*Vector2f policemanDir = p_policeman->GetDirection();
		if (policemanDir.GetX() > 0)
			p_policeman->SetAnimationByName("WalkRight");
		else if (policemanDir.GetX() < 0)
			p_policeman->SetAnimationByName("WalkLeft");

		if (policemanDir.GetY() > 0)
			p_policeman->SetAnimationByName("WalkDown");
		else if (policemanDir.GetY() < 0)
			p_policeman->SetAnimationByName("WalkUp");*/

		Pathfinder pathfinder;

		Vector2i policemanGridPos = { (int)p_policeman->GetPosition().GetX() / 16, (int)p_policeman->GetPosition().GetY() / 16 };
		Vector2i playerGridPos = { (int)p_policeman->GetPlayerPos().GetX() / 16, (int)p_policeman->GetPlayerPos().GetY() / 16 };

		int pathIndex = p_policeman->GetPathIndex();

		std::vector<Tile*> path = p_policeman->GetPath();

		Vector2f dir = { path[pathIndex]->gridPos.GetX() - policemanGridPos.GetX(),
			path[pathIndex]->gridPos.GetY() - policemanGridPos.GetY() };

		p_policeman->SetDirection(dir);

		if (policemanGridPos == path[pathIndex]->gridPos)
		{
			pathIndex++;
			p_policeman->SetPathIndex(pathIndex);

			if (pathIndex % 2 == 0)
			{
				p_policeman->SetPath(pathfinder.FindPath(*p_policeman->GetScene<GameScene>()->GetNavigationGrid(),
					policemanGridPos, playerGridPos));
			}
		}
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