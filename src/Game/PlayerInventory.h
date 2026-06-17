#pragma once

#include "ItemID.h"
#include "ItemStack.h"

class Hotbar;
class Inventory;
class InventorySlot;

class PlayerInventory
{
	Inventory* m_inventory = nullptr;

	Hotbar* m_hotbar = nullptr;

	bool m_isInventoryOpen = false;

	InventorySlot* m_selectedSlot = nullptr;

public:

	PlayerInventory(Inventory* inv, Hotbar* hot);



	void Open();
	void Close();
	void Toggle();

	bool IsOpen();



	bool CanAdd(ItemID ID, int amount);
	bool CanRemove(ItemID ID, int amount);
	int AddItem(ItemID ID, int amount, bool requireFull);
	int RemoveItem(ItemID ID, int amount, bool requireFull);



	int CountItem(ItemID ID);

	bool HasItem(ItemID ID);
	bool HasItems(ItemID ID, int amount);

	bool IsEmpty();



	Inventory* GetInventory();
	Hotbar* GetHotbar();
	void Select(int index);
	void GoNext(int dir);
	int GetSelectedIndex();
	ItemStack& GetSelectedStack();
	ItemID GetSelectedItemID();
	InventorySlot* GetInventorySlot(int index);
	InventorySlot* GetHotbarSlot(int index);



	void Clear();



	void OnUpdate();
};