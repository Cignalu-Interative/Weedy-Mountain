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

	bool m_inventoryOpen = false;

public:

	Hotbar(int slotcount = 10, int invSlotCount = 30);

	void OnInitialize() override;
	void OnUpdate() override;

	bool CanAdd(const std::string& ID, int amount);
	bool CanRemove(const std::string& ID, int amount);
	int AddItem(const std::string& ID, int amount, bool requireFull);
	int RemoveItem(const std::string& ID, int amount, bool requireFull);

	int CountItem(const std::string& ID);

	InventorySlot* GetSlot(int index);

	std::vector<InventorySlot*>& GetSlots();

	void Select(int index);
	void /*FF*/GoNext(int dir);

	int GetSelectedIndex();
	ItemStack& GetSelectedStack();
	const std::string& GetSelectedItemID();

	void RealignSlots();

	void SetIsInventoryOpen(bool inventory);
	bool GetIsInventoryOpen();
};