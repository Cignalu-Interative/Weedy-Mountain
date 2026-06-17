#pragma once
#include "Entity.h"
#include "InventorySlot.h"
#include "Button.h"

class Vendor : public Entity
{

private :

	InventorySlot* m_slot = nullptr;

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

