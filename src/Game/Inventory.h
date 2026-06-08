#pragma once

#include "InventorySlot.h"
#include <vector>

class Inventory
{
private:

	std::vector<InventorySlot> m_slots;

public:

	Inventory(int slotCount = 40);


	bool CanAdd(ItemID ID, int amount);
	bool CanRemove(ItemID ID, int amount);
	int AddItem(ItemID ID, int amount, bool requireFull);
	int RemoveItem(ItemID ID, int amount, bool requireFull);

	int CountItem(ItemID ID);

	InventorySlot& GetSlot(int index);

	std::vector<InventorySlot>& GetSlots();

	int GetSlotCount();
};