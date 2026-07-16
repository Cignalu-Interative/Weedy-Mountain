#include "pch.h"
#include "Policeman.h"

#include "PolicemanState.h"
#include "PolicemanCondition.h"

void Policeman::OnInitialize()
{
	AddCollider("detection", { 80, 80 }, Shape::Circle, { 0.f, 0.f }, true);
	m_navigationGrid.Initialize();
}

void Policeman::OnUpdate()
{
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
		m_isDetectedPlayer = true;
	}
}

void Policeman::OnCollisionExit(Collider* p_self, Collider* p_other, CollisionSide p_side)
{
	if (p_self->GetID() == "detection" && p_other->GetOwner()->GetTag() == 1)
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
	}

	//PATROL 
	{
		// to chase
		auto transition = m_stateMachine.AddTransition((int)PolicemanState::PATROL, (int)PolicemanState::CHASE);
		transition->AddCondition(new HasDetectedPlayer);
	}

	//CHASE
	{
		auto transition = m_stateMachine.AddTransition((int)PolicemanState::CHASE, (int)PolicemanState::IDLE);
		transition->AddCondition(new HasCollideWithPlayer);

		transition->AddCondition(new HasLoosePlayer);
	}
}

void Policeman::StartChase()
{
	m_navigationGrid.SetTileCrossedByType(TileType::GRASS, true);
}

void Policeman::StartPatrol()
{
	m_navigationGrid.SetTileCrossedByType(TileType::GRASS, false);
}


