#pragma once
#include "Condition.h"
#include "Policeman.h"

class IdleCooldownEnded : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{

	}
};

class HasDetectedPlayer : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{

	}
};

class HasCollideWithPlayer : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{

	}
};

class HasLoosePlayer : public Condition<Policeman>
{
	bool TestBool(Policeman* p_policeman) override
	{

	}
};