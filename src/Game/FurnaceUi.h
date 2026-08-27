#pragma once
#include "Entity.h"
#include "2DpendentLib/json.hpp"

class PlayerInventory;
class InventorySlot;
class Animation;
class FurnaceUi : public Entity
{
private :

	PlayerInventory* m_playerInventory = nullptr;

	InventorySlot* m_rawOres = nullptr;
	InventorySlot* m_meltedOres = nullptr;
	InventorySlot* m_consumables = nullptr;

	Animation* m_progressBarAnimation = nullptr;
	Animation* m_flameAnimation = nullptr;

	float m_fuelTime = 10.f; // time one of wood give  
	float m_currentfuelTime = 0;

	float m_heatingTimeRequired = 0.f;// time for item in rawOres
	float m_currentHeatingTime = m_heatingTimeRequired;

	bool m_isOpen = false;

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;

	void PlaceItemInRawOresSlot();
	void PlaceItemInConsumablesSlot();

	void PlaceItemInFurnaceSlot(InventorySlot* p_slot);
	void PlaceItemInFurnaceSlot(const std::string& p_idToTest, InventorySlot* p_slot);
	void GetMeltedOres();

	void PlaceItemInInventory(InventorySlot* p_slot);

public : 

	void Open();
	void Close();

	void Save(nlohmann::json& p_out) const;
	void Load(const nlohmann::json& p_in);

	bool GetIsOpen() { return m_isOpen; }

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });
};

