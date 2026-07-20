#pragma once
#include "PNJ.h"

class Inventory;
class shopPNJ : public PNJ
{
private : 
	Inventory* m_inventory = nullptr;

public : 
	void OnInitialize() override; 
	void OnUpdate() override; 


	Inventory* GetInventory() { return m_inventory; }
};

