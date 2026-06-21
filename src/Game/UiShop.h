#pragma once
#include "Entity.h"

#include "Inventory.h"

class UiShop : public Entity
{

private :

	InventorySlot* m_slot = nullptr;

	Inventory* m_shopInventory = nullptr;
	Inventory* m_playerInventory = nullptr;

	Button* m_sellButton = nullptr;
	Button* m_buyButton = nullptr;

	Button* m_plusButton = nullptr;
	Button* m_minusButton = nullptr;

	Button* m_choosePlayerInventory = nullptr;
	Button* m_chooseVendorInventory = nullptr;

	//ne pas mettre en variable membre
	std::string m_quantitesItem;
	std::string m_totalPrice;

	int m_amountOfItem = 0;
	int m_totalPrices = 0;

public : 

	void OnInitialize() override;

	// place playerInventory in the shop
	void PlaceInventory(Inventory* p_inventory);
};

