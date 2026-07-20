#include "pch.h"
#include "shopPNJ.h"
#include "Inventory.h"

void shopPNJ::OnInitialize()
{
	PNJ::OnInitialize();
	m_inventory = GetScene()->CreateEntity<Inventory>(GetPosition(), { 636, 210 }, "InventoryWeedyMountain", Shape::Rectangle, true);

	m_inventory->SetDrawLayer(INT_MAX - 4);
	m_inventory->RemoveCollider("default");

}
void shopPNJ::OnUpdate()
{

}


