#pragma once

#include "Entity.h"
#include "InventorySlot.h"
#include <vector>

#include "Savable.h"

class Hotbar : public Entity, public Savable
{
	std::vector<InventorySlot*>  m_slots;

	int m_slotCount;
	int m_invSlotCount;

	int m_selectedIndex = 0;

	bool m_inventoryOpen = false;

public:

	Hotbar(int slotcount = 10, int invSlotCount = 30);

	void OnInitialize() override;
	void OnDestroy() override;
	void OnUpdate() override;
	void SetPosition(Vector2f p_newPos, Vector2f p_anchor = { 0.5f, 0.5f });
	void SetScene(Scene* p_newScene);
	void SetIsVisible(bool p_isVisible);

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

	std::string GetSaveID() const override { return "player_hotbar"; }
	void Save(nlohmann::json& p_out) override;
	void Load(const nlohmann::json& p_in) override;
	void RealignSlots();

	void SetIsInventoryOpen(bool inventory);
	bool GetIsInventoryOpen();
};