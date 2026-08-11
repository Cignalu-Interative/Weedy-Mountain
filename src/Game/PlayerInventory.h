#pragma once

#include "ItemID.h"
#include "ItemStack.h"

class Hotbar;
class Inventory;
class InventorySlot;
class NotificationManager;

class PlayerInventory
{
	Inventory* m_inventory = nullptr;

	Hotbar* m_hotbar = nullptr;

	NotificationManager* m_notifManager = nullptr;

	bool m_isInitialized = false;

public:

	PlayerInventory() = default;

	PlayerInventory(Inventory* inv, Hotbar* hot);


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
	int GetSelectedIndex();
	ItemStack& GetSelectedStack();
	const std::string& GetSelectedItemID();
	InventorySlot* GetInventorySlot(int index);
	InventorySlot* GetHotbarSlot(int index);

	std::vector<InventorySlot*> GetAllSlots();

	void Clear();


	void SetNotificationManager(NotificationManager* notificationManager);

};