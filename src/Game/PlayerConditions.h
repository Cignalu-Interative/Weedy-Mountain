#pragma once
#include "Condition.h"
#include "Player.h"

class HasSpadeInHands : public Condition<Player>
{
	bool TestBool(Player* p_player)
	{
		// check if current item in hands is spade
		return true;
	}
};

