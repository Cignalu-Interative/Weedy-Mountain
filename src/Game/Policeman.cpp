#include "pch.h"
#include "Policeman.h"

#include "PolicemanState.h"
#include "PolicemanCondition.h"

void Policeman::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);
	AddCollider("detection", { 80, 80 }, Shape::Circle, { 0.f, 0.f }, true);
	AddCollider("vision", { 300, 300 }, Shape::Circle, { 0.f, 0.f }, true);

	for (Collider* col : GetColliders())
	{
		col->SetShow(true);
	}

	SetSpeed(50);

	m_navigationGrid = new NavigationGrid;

	SetupStateMachine();
}

void Policeman::OnUpdate()
{
	if (m_navigationGrid->GetMapTiles().size() <= 0)
	{
		m_navigationGrid->Initialize();
	}

	m_stateMachine.Update(this, GetDeltaTime());
	MoveKinematic();
}

void Policeman::OnEvent()
{
}

void Policeman::OnCollisionStay(Collider* p_self, Collider* p_other, CollisionSide p_side)
{
	if (p_self->GetID() == "default" && p_other->GetOwner()->GetTag() == 1 && p_other->GetID() == "default")
	{
		m_isCollidingWithPlayer = true;
	}

	if (m_isUnreachable)
	{
		m_isPlayerDetected = false;
		m_isLoosingPlayer = true;
		return;
	}

	if (p_other->GetOwner()->GetTag() == 1 && p_self->GetID() == "detection")
	{
		if (m_isPlayerDetected == false && m_isUnreachable == false)
		{
			m_isPlayerDetected = true;
			m_isLoosingPlayer = false;
			SetState(PolicemanState::CHASE);
		}

		m_playerPos = p_other->GetOwner()->GetPosition();
	}
	if (p_other->GetOwner()->GetTag() == 1 && p_other->GetID() == "default" && p_self->GetID() == "vision" &&
		IsInVisionCone(p_other->GetPosition()))
	{
		if (m_isPlayerDetected == false && m_isUnreachable == false)
		{
			m_isPlayerDetected = true;
			m_isLoosingPlayer = false;

			if (m_stateMachine.GetState() != (int)PolicemanState::CHASE)
			{
				SetState(PolicemanState::CHASE);
			}
		}
		m_playerPos = p_other->GetOwner()->GetPosition();
	}
}

void Policeman::OnCollisionExit(Collider* p_self, Collider* p_other, CollisionSide p_side)
{
	if (p_self->GetID() == "default" && p_other->GetOwner()->GetTag() == 1 && p_other->GetID() == "default")
	{
		m_isCollidingWithPlayer = false;
	}
	if (p_other->GetOwner()->GetTag() == 1 && p_self->GetID() == "vision")
	{
		m_isLoosingPlayer = true;
		m_isPlayerDetected = false;
		m_isUnreachable = false;
	}
	if (IsInVisionCone(p_other->GetPosition()) == false && p_self->GetID() == "detection")
	{
		m_isLoosingPlayer = true;
		m_isPlayerDetected = false;
		m_isUnreachable = false;
	}
}

void Policeman::SetupStateMachine()
{
	m_stateMachine.AddState(new PolicemanIdleState);//0
	m_stateMachine.AddState(new PolicemanPatrolState);//1
	m_stateMachine.AddState(new PolicemanChaseState);//2

	//TRANSITIONS 

	//IDLE
	{
		// to patrol
		auto transition = m_stateMachine.AddTransition((int)PolicemanState::IDLE, (int)PolicemanState::PATROL);
		transition->AddCondition(new HasIdleCooldownEnded);

		// to chase
		transition = m_stateMachine.AddTransition((int)PolicemanState::IDLE, (int)PolicemanState::CHASE);
		transition->AddCondition(new HasDetectedPlayer);
	}

	//PATROL 
	{
		// to chase
		auto transition = m_stateMachine.AddTransition((int)PolicemanState::PATROL, (int)PolicemanState::CHASE);
		transition->AddCondition(new HasDetectedPlayer);

		// to idle
		transition = m_stateMachine.AddTransition((int)PolicemanState::PATROL, (int)PolicemanState::IDLE);
		transition->AddCondition(new HasArrived);
	}

	//CHASE
	{
		// to idle
		auto transition = m_stateMachine.AddTransition((int)PolicemanState::CHASE, (int)PolicemanState::IDLE);
		transition->AddCondition(new HasCollideWithPlayer);

		auto transition2 = m_stateMachine.AddTransition((int)PolicemanState::CHASE, (int)PolicemanState::IDLE);
		transition2->AddCondition(new HasLoosePlayer);
	}

	SetState(PolicemanState::IDLE);
}

void Policeman::StartChase()
{
	m_navigationGrid->SetTileCrossedByType(TileType::GRASS, true);

	m_pathIndex = 0;

	Vector2i start = { (int)(m_position.GetX() / 16), (int)(m_position.GetY() / 16) };
	Vector2i goal = { (int)m_playerPos.GetX() / 16, (int)m_playerPos.GetY() / 16 };

	m_path = m_pathfinder.FindPath(*m_navigationGrid, start, goal);
}

void Policeman::StartPatrol()
{
	m_pathIndex = 0;

	Vector2i start = { (int)(m_position.GetX() / 16), (int)(m_position.GetY() / 16) };
	int goalIndex = Aleatory::GetInt(0, m_navigationGrid->GetTilesByType(TileType::PATH).size());
	Vector2i goalPos = m_navigationGrid->GetTilesByType(TileType::PATH)[goalIndex]->gridPos;

	if (m_navigationGrid->GetTileByGridPos(start)->type == TileType::PATH)
	{
		m_navigationGrid->SetTileCrossedByType(TileType::GRASS, false);
	}

	m_path = m_pathfinder.FindPath(*m_navigationGrid, start, goalPos);

	int maxAttempts = 10;
	int attempts = 0;

	do
	{
		int goalIndex = Aleatory::GetInt(0, m_navigationGrid->GetTilesByType(TileType::PATH).size());
		Vector2i goalPos = m_navigationGrid->GetTilesByType(TileType::PATH)[goalIndex]->gridPos;

		m_path = m_pathfinder.FindPath(*m_navigationGrid, start, goalPos);

		attempts++;
	} while (m_path.size() == 0 && attempts < maxAttempts);

	m_navigationGrid->SetTileCrossedByType(TileType::GRASS, false);
}

bool Policeman::IsInVisionCone(Vector2f p_goalPos)
{
	Vector2f dir = p_goalPos - m_position;

	float dirLength = std::sqrt(dir.GetX() * dir.GetX() + dir.GetY() * dir.GetY());

	if (dirLength == 0)
	{
		return false;
	}

	Vector2f normalizeDir = dir / dirLength;

	float dotDir;

	if (m_facingDir.GetX() == 0.f && m_facingDir.GetY() == 0.f)
	{
		dotDir = Utils::Dot(m_facingDir, normalizeDir);
	}
	else
	{
		dotDir = Utils::Dot(Utils::NormalizeVector(m_facingDir), normalizeDir);
	}

	if (dotDir >= m_limitedCos)
	{
		return true;
	}

	return false;
}

void Policeman::SetState(PolicemanState p_state)
{
	m_stateMachine.TryTransitionTo(this, (int)p_state);
}