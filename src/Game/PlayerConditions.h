#pragma once
#include "Condition.h"
#include "Player.h"

class HasSpadeInHands : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		// check if current item in hands is spade
		
		//return p_player->hasSpade;
		return true;
	}
};

class HasPickaxeInHands : public Condition<Player>
{
    bool TestBool(Player* p_player) override
    {
        // check if current item in hands is pickaxe
        //return p_player->hasPickaxe;
		return true;
    }
};

class HasWateringCanInHands : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		// check if current item in hands is Watering Can
		//return p_player->hasWateringCan;.
		return true;
	}
};

class IsPlayerGoingDown : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		return p_player->GetDirection().GetY() < 0;
	}
};

class IsPlayerGoingUp : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		return p_player->GetDirection().GetY() > 0;
	}
};

class IsPlayerGoingLeft : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		return p_player->GetDirection().GetX() < 0;
	}
};

class IsPlayerGoingRight : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		return p_player->GetDirection().GetX() > 0;
	}
};

class IsPlayerStatic : public Condition<Player>
{
	bool TestBool(Player* p_player)
	{
		return p_player->GetDirection().GetX() == 0 && p_player->GetDirection().GetY() == 0;
	}
};

class IsPlayerUsingToolInHand : public Condition<Player>
{
	bool TestBool(Player* p_player)
	{
		return p_player->IsUsingToolInHand();
	}
};

class CanPlayerDig : public Condition<Player>
{
	bool TestBool(Player* p_player)
	{
		//return p_player->IsUsingToolInHand() && p_player->hasSpade;
	}
};