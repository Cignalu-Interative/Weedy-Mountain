#pragma once

#include "InventorySlot.h"
#include <vector>
#include "Entity.h"
#include "Savable.h"

class Inventory : public Entity, public Savable
{
private:

	std::vector<InventorySlot*> m_slots;

	int m_slotCount;

	uint32_t m_myID;
	std::string m_saveTag;

public:

	Inventory(int slotCount = 30);

	void OnInitialize() override;
	void OnDestroy() override;

	void SetSaveTag(const std::string& p_tag) { m_saveTag = p_tag; }

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

	std::string GetSaveID() const override
	{
		return m_saveTag.empty() ? ("inventory_" + std::to_string(m_myID)) : m_saveTag;
	}

	void Save(nlohmann::json& p_out) override;
	void Load(const nlohmann::json& p_in) override;
};