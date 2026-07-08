#pragma once
#include "PNJ.h"
#include "StateMachine.h"

enum class PolicemanState
{
	IDLE,
	PATROL,
	CHASE,

	COUNT
};

class Policeman : public PNJ
{
private:
	
	bool m_isDetectedPlayer = false;
	bool m_isLoosingPlayer = false;

	bool m_isIdleTimerEnded = false;

	bool m_isCollidingWithPlayer = false;

	StateMachine m_stateMachine;
	

protected:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;
	void OnCollisionStay(Collider* p_self, Collider* p_other, CollisionSide p_side) override;
	void OnCollisionExit(Collider* p_self, Collider* p_other, CollisionSide p_side) override;

public:

	void SetupStateMachine();

	void SetIdleCooldownEnded(bool p_value) { m_isIdleTimerEnded = p_value; }

	bool IsDetectedPlayer() { return m_isDetectedPlayer; }
	bool IsIdleTimerEnded() { return m_isIdleTimerEnded; }
	bool IsLoosingPlayer() { return m_isLoosingPlayer; }
	bool IsCollidingWithPlayer() { return m_isCollidingWithPlayer; }

};

