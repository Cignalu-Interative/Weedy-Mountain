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

	bool m_isInitialized = false;

public:

	PlayerInventory() = default;

	PlayerInventory(Inventory* inv, Hotbar* hot);


	void Open();
	void Close();
	void Toggle();

	bool IsOpen();
	bool IsInitialized() { return m_isInitialized; }

	bool CanAdd(const std::string& ID, int amount);
	bool CanRemove(const std::string& ID, int amount);
	int AddItem(const std::string& ID, int amount, bool requireFull);
	int RemoveItem(const std::string& ID, int amount, bool requireFull);

	int CountItem(const std::string& ID);

	bool HasItem(const std::string& ID);
	bool HasItems(const std::string& ID, int amount);

	bool IsEmpty();

	Inventory* GetInventory();
	Hotbar* GetHotbar();
	void Select(int index);
	void GoNext(int dir);
	int GetSelectedIndex();
	ItemStack& GetSelectedStack();
	const std::string& GetSelectedItemID();
	InventorySlot* GetInventorySlot(int index);
	InventorySlot* GetHotbarSlot(int index);

	void Clear();

	void OnUpdate();

};