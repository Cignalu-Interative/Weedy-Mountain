#pragma once

#include "ItemID.h"
#include <string>

class ItemData
{
private:
	
	ItemID m_ID = ItemID::ITEM_NOTHING;

	std::string m_path;

	std::string m_name;
	std::string m_description;

	int m_buyPrice = 0;
	int m_sellPrice = 0;

	int m_maxStack = 64;

public:

	ItemData(ItemID ID, std::string path, std::string name, std::string description, int buyPrice, int sellPrice, int maxStack);

	ItemID GetID();
	std::string& GetPath();
	std::string& GetName();
	std::string& GetDescription();
	int GetBuyPrice();
	int GetSellPrice();
	int GetMaxStack();
};