#pragma once
#include "Condition.h"
#include "Player.h"

class HasSpadeInHands : public Condition<Player>
{
	bool TestBool(Player* p_player)
	{
		// check if current item in hands is spade
		std::cout << "trou" << std::endl;
		return true;
	}
};

class HasPickaxeInHands : public Condition<Player>
{
	bool TestBool(Player* p_player)
	{
		// check if current item in hands is pickaxe
		std::cout << "false" << std::endl;
		return false;
	}
};

class HasWateringCanInHands : public Condition<Player>
{
	bool TestBool(Player* p_player)
	{
		// check if current item in hands is Watering Can
		std::cout << "false" << std::endl;
		return false;
	}
};

