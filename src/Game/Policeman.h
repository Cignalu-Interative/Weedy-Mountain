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
	
	bool m_isPlayerDetected = false;

	

protected:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

public:

	void SetupStateMachine();

};

