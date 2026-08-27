#pragma once
#include "Entity.h"

class PlayerInventory;
class Inventory;
class UiChest :public Entity
{

private : 

	PlayerInventory* m_playerInventory = nullptr;
	
	Inventory* m_chestInventory = nullptr;

	bool m_isOpen = false;

	void OnInitialize() override;
	void OnEvent() override;

	void AddItemInChest();
	void AddItemInPlayerInventory();

public:

	void Open();
	void Close(); 

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });

	bool GetIsOpen() { return m_isOpen; }
};

