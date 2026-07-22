#pragma once
#include "Condition.h"
#include "Policeman.h"

#include "GameScene.h"

class HasIdleCooldownEnded : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{
		return p_policeman->IsIdleCooldownEnded();
	}
};

class HasDetectedPlayer : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{
		return p_policeman->IsPlayerDetected();
	}
};

class HasCollideWithPlayer : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{
		return p_policeman->IsCollidingWithPlayer();
	}
};

class HasLoosePlayer : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{
		return p_policeman->IsLoosingPlayer();
	}
};

class HasArrived : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{
		return p_policeman->IsArrived();
	}
};