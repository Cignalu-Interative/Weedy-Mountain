#pragma once

#include "InventorySlot.h"
#include <vector>
#include "Entity.h"

class Inventory : public Entity
{
private:

	std::vector<InventorySlot*> m_slots;

public:

	Inventory(int slotCount = 40);


	bool CanAdd(ItemID ID, int amount);
	bool CanRemove(ItemID ID, int amount);
	int AddItem(ItemID ID, int amount, bool requireFull);
	int RemoveItem(ItemID ID, int amount, bool requireFull);

	int CountItem(ItemID ID);

	void SetVisible(bool visible);

	InventorySlot* GetSlot(int index);

	std::vector<InventorySlot*>& GetSlots();

	int GetSlotCount();
};