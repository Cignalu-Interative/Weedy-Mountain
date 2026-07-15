#pragma once

#include "InventorySlot.h"
#include <vector>
#include "Entity.h"

class Inventory : public Entity
{
private:

	std::vector<InventorySlot*> m_slots;

	int m_slotCount;

public:

	Inventory(int slotCount = 30);

	void OnInitialize() override;

	bool CanAdd(const std::string& ID, int amount);
	bool CanRemove(const std::string& ID, int amount);
	int AddItem(const std::string& ID, int amount, bool requireFull);
	int RemoveItem(const std::string& ID, int amount, bool requireFull);

	int CountItem(const std::string& ID);

	void SetVisible(bool visible);

	InventorySlot* GetSlot(int index);

	std::vector<InventorySlot*>& GetSlots();

	int GetSlotCount();

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });
	void SetScene(Scene* p_newScene);

	bool HasAmountOfItem(const std::string & p_ID, int p_amount);
};