#pragma once
#include "StateBase.h"
#include "Policeman.h"
#include "GameScene.h"

class PolicemanIdleState : public StateBase<Policeman>
{
private:

	float m_cooldown = 0.f;

public:

	void Start(Policeman* p_policeman) {
		p_policeman->SetAnimationByName("Idle");

		m_cooldown = 1.5f;
		p_policeman->SetIdleCooldownEnded(false);
		p_policeman->SetIsLoosingPlayer(false);
		p_policeman->SetIsPlayerDetected(false);
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
		std::vector<NavTile*> path = p_policeman->GetPath();

		if (pathIndex >= path.size())
		{
			p_policeman->SetState(PolicemanState::IDLE);
			return;
		}

		Vector2i policemanGridPos = { (int)p_policeman->GetPosition().GetX() / 16, (int)p_policeman->GetPosition().GetY() / 16 };

		Vector2f dir = { (float)path[pathIndex]->gridPos.GetX() - (float)policemanGridPos.GetX(),
			(float)path[pathIndex]->gridPos.GetY() - (float)policemanGridPos.GetY() };

		p_policeman->SetDirection(dir);
		p_policeman->SetFacingDir(dir);

		if (policemanGridPos == path[pathIndex]->gridPos)
		{
			pathIndex++;
			p_policeman->SetPathIndex(pathIndex);
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
			return false;
			break;
		}
	};
};

class PolicemanChaseState : public StateBase<Policeman>
{
	void Start(Policeman* p_policeman) 
	{
		p_policeman->SetIsLoosingPlayer(false);

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

		if (p_policeman->IsUnreachable())
		{
			return;
		}

		Pathfinder pathfinder;
		GameScene* gs = p_policeman->GetScene<GameScene>();

		Vector2i policemanGridPos = { (int)p_policeman->GetPosition().GetX() / 16, (int)p_policeman->GetPosition().GetY() / 16 };
		Vector2i playerGridPos = { (int)p_policeman->GetPlayerPos().GetX() / 16, (int)p_policeman->GetPlayerPos().GetY() / 16 };

		int pathIndex = p_policeman->GetPathIndex();

		std::vector<NavTile*> path = p_policeman->GetPath();

		if (pathIndex >= path.size())
		{
			path = pathfinder.FindPath(*p_policeman->GetNavigationGrid(), policemanGridPos, playerGridPos);
			
			if (path.size() == 0)
			{
				p_policeman->SetIsUnreachable(true);
				p_policeman->SetIsLoosingPlayer(true);
				p_policeman->SetIsPlayerDetected(false);

				p_policeman->SetState(PolicemanState::IDLE);
				return;
			}

			pathIndex = 0;
			p_policeman->SetPathIndex(pathIndex);
			p_policeman->SetPath(path);
		}

		Vector2f dir = { (float)path[pathIndex]->gridPos.GetX() - (float)policemanGridPos.GetX(),
			(float)path[pathIndex]->gridPos.GetY() - (float)policemanGridPos.GetY() };

		p_policeman->SetDirection(dir);
		p_policeman->SetFacingDir(dir);

		if (policemanGridPos == path[pathIndex]->gridPos)
		{
			pathIndex++;
			p_policeman->SetPathIndex(pathIndex);

			if (pathIndex % 2 == 0)
			{
				NavigationGrid nav; // A ENLEVER
				p_policeman->SetPath(pathfinder.FindPath(nav/**gs->GetNavigationGrid()*/,
					policemanGridPos, playerGridPos));

				p_policeman->SetPathIndex(0);
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
			return true;
			break;
		}
	};
};

