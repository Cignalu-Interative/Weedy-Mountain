#pragma once

#include "Entity.h"
#include "InventorySlot.h"
#include <vector>

class Hotbar : public Entity
{
	std::vector<InventorySlot*>  m_slots;

	int m_slotCount;
	int m_invSlotCount;

	int m_selectedIndex = 0;

public:

	Hotbar(int slotcount = 10, int invSlotCount = 30);

	void OnInitialize() override;
	void OnUpdate() override;

	bool CanAdd(ItemID ID, int amount);
	bool CanRemove(ItemID ID, int amount);
	int AddItem(ItemID ID, int amount, bool requireFull);
	int RemoveItem(ItemID ID, int amount, bool requireFull);

	int CountItem(ItemID ID);

	InventorySlot* GetSlot(int index);

	std::vector<InventorySlot*>& GetSlots();

	void Select(int index);
	void /*FF*/GoNext(int dir);

	int GetSelectedIndex();
	ItemStack& GetSelectedStack();
	ItemID GetSelectedItemID();
};