#pragma once
#include "PNJ.h"

#include "Inventory.h"

class UiShop : public PNJ
{

private :

	InventorySlot* m_slot = nullptr;

	Inventory* m_inventory = nullptr;

	Button* m_sellButton = nullptr;
	Button* m_buyButton = nullptr;

	Button* m_plusButton = nullptr;
	Button* m_minusButton = nullptr;

	Button* m_playerInventory = nullptr;
	Button* m_vendorInventory = nullptr;

	//ne pas mettre en variable membre
	std::string m_quantitesItem;
	std::string m_totalPrice;

public : 

	void OnInitialize() override;


};

