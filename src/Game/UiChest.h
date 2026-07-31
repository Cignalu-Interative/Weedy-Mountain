#pragma once
#include "Entity.h"

class PlayerInventory;
class Inventory;
class UiChest :public Entity
{

private : 

	PlayerInventory* m_playerInventory = nullptr;
	
	Inventory* m_chestInventory = nullptr;

	void OnInitialize() override;
	void OnEvent() override;

	void AddItemInChest();
	void AddItemInPlayerInventory();

public:

	void Open();
	void Close(); 
};

