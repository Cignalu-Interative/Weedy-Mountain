#pragma once
#include "2DPendentEngine/Entity.h"
#include "ItemID.h"

class ItemStack
{
private:

	ItemID m_IDD = ITEM_NOTHING;

	int m_amount = 0;

public:

	ItemStack(ItemID ID, int amount);

	bool IsEmpty();
	void Clear();

	ItemID GetID();
	void SetID(ItemID newID);

	int GetAmount();
	void SetAmount(int newAmount);
};