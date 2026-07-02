#pragma once
#include "Condition.h"
#include "Player.h"
#include "PlayerInventory.h"

class HasSpadeInHands : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		// check if current item in hands is spade
		std::string currentItemID = p_player->GetInventory()->GetSelectedItemID();
		if (currentItemID == "ITEM_SPADE")
			return true;

		return false;
	}
};

class HasPickaxeInHands : public Condition<Player>
{
    bool TestBool(Player* p_player) override
    {
		std::string currentItemID = p_player->GetInventory()->GetSelectedItemID();
		if (currentItemID == "ITEM_PICKAXE")
			return true;

		return false;
    }
};

class HasWateringCanInHands : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		std::string currentItemID = p_player->GetInventory()->GetSelectedItemID();
		if (currentItemID == "ITEM_WATERING_CAN")
			return true;

		return false;
	}
};

class IsPlayerStatic : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		return p_player->GetDirection().GetX() == 0 && p_player->GetDirection().GetY() == 0;
	}
};

class IsPlayerUsingToolInHand : public Condition<Player>
{
	bool TestBool(Player* p_player) override
	{
		return p_player->IsUsingToolInHand();
	}
};
