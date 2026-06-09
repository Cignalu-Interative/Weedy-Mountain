#pragma once

#include "ItemData.h"
#include <unordered_map>

class ItemDataBase
{
private:

	std::unordered_map<ItemID, ItemData> m_items;

public:

	static ItemDataBase* Get();

	ItemDataBase();

	ItemData* GetItem(ItemID ID);
};