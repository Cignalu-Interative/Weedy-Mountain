#pragma once
#include "Condition.h"
#include "Policeman.h"

#include "GameScene.h"

class HasIdleCooldownEnded : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{
		
	}
};

class HasDetectedPlayer : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{
		return p_policeman->IsDetectedPlayer();
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