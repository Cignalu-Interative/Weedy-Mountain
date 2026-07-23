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

	SetSpeed(500);

	SetupStateMachine();
}

void Policeman::OnUpdate()
{
	MoveKinematic();

	m_stateMachine.Update(this, GetDeltaTime());
}

void Policeman::OnEvent()
{
}

void Policeman::OnCollisionStay(Collider* p_self, Collider* p_other, CollisionSide p_side)
{
	if (p_other->GetOwner()->GetTag() == 1 && p_self->GetID() == "default")
	{
		m_isCollidingWithPlayer = true;
	}
	if (p_other->GetOwner()->GetTag() == 1 && p_self->GetID() == "detection")
	{
		m_isPlayerDetected = true;
		m_playerPos = p_other->GetOwner()->GetPosition();
	}
	if (p_other->GetOwner()->GetTag() == 1 && p_other->GetID() == "default" && p_self->GetID() == "vision" &&
		IsInVisionCone(p_other->GetPosition()))
	{
		m_isPlayerDetected = true;
		m_playerPos = p_other->GetOwner()->GetPosition();
	}
}

void Policeman::OnCollisionExit(Collider* p_self, Collider* p_other, CollisionSide p_side)
{
	if (p_self->GetID() == "detection" && p_other->GetOwner()->GetTag() == 1)
	{
		m_isLoosingPlayer = true;
	}
	if (p_other->GetOwner()->GetTag() == 1 && p_self->GetID() == "vision" &&
		IsInVisionCone(p_other->GetPosition()))
	{
		m_isLoosingPlayer = true;
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

		transition->AddCondition(new HasLoosePlayer);
	}

	SetState(PolicemanState::IDLE);
}

void Policeman::StartChase()
{
	NavigationGrid* navigationGrid = GetScene<GameScene>()->GetNavigationGrid();

	navigationGrid->SetTileCrossedByType(TileType::GRASS, true);

	m_pathIndex = 0;

	Vector2i start = { (int)(m_position.GetX() / 16), (int)(m_position.GetY() / 16) };
	Vector2i goal = { (int)m_playerPos.GetX() / 16, (int)m_playerPos.GetY() / 16 };

	m_path = m_pathfinder.FindPath(*navigationGrid, start, goal);
}

void Policeman::StartPatrol()
{
	NavigationGrid* navigationGrid = GetScene<GameScene>()->GetNavigationGrid();

	navigationGrid->SetTileCrossedByType(TileType::GRASS, false);

	m_pathIndex = 0;

	Vector2i start = { (int)(m_position.GetX() / 16), (int)(m_position.GetY() / 16) };
	int goalIndex = Aleatory::GetInt(0, navigationGrid->GetTilesByType(TileType::PATH).size());
	Vector2i goalPos = navigationGrid->GetTilesByType(TileType::PATH)[goalIndex]->gridPos;

	m_path = m_pathfinder.FindPath(*navigationGrid, start, goalPos);
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

	float dotDir = Utils::Dot(Utils::NormalizeVector(m_direction), normalizeDir);

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