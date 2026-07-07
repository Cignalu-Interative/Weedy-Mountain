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
	Button* m_sellAllButton = nullptr;
	Button* m_buyButton = nullptr;

	Button* m_plusButton = nullptr;
	Button* m_minusButton = nullptr;

	Button* m_choosePlayerInventory = nullptr;
	Button* m_chooseShopInventory = nullptr;

	int m_slotIndex = 0;

	int m_amountOfItem = 0;
	int m_totalPrices = 10;

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	// place Inventory in the shop
	void PlaceInventory(Inventory* p_inventory);

	// check if the inventory of the player has place and if the player can have enough money to buy the item
	void Buy();

	// check if the inventory of the player has the item
	void Sell();

	void SellAll();

	void ChoosePlayerInventoryButtonClicked();

	void ChooseShopInventoryButtonClicked();

	void PlaceItemInInventoryToSlot();

	void PlusButton();

	void MinusButton();

public : 

	void SetShopInventory(Inventory* p_shopInventory) { m_shopInventory = p_shopInventory; }

	void SetVisible(bool p_isVisible);
};

