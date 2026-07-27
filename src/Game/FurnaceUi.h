#pragma once
#include "Entity.h"

class PlayerInventory;
class InventorySlot;
class FurnaceUi : public Entity
{
private :

	PlayerInventory* m_playerInventory = nullptr;

	InventorySlot* m_rawOres = nullptr;
	InventorySlot* m_meltedOres = nullptr;
	InventorySlot* m_consumables = nullptr;

	float m_fuelTime = 10.f; // time one of wood give  
	float m_currentfuelTime = 0;

	float m_heatingTimeRequired = 10.f;// time for item in rawOres
	float m_currentHeatingTime = m_heatingTimeRequired;

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;

	void PlaceItemInRawOresSlot();
	void PlaceItemInConsumablesSlot();

	void PlaceItemInFurnaceSlot(const std::string& p_idToTest, InventorySlot* p_slot);
	void GetMeltedOres();

	void PlaceItemInInventory(InventorySlot* p_slot);

public : 

	void Open();
	void Close();
};

