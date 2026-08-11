#include "pch.h"
#include "PlayerInventory.h"
#include "Inventory.h"
#include "Hotbar.h"
#include "InventorySlot.h"
#include "ItemDataBase.h"
#include "2DPendentLib/InputManager.h"
#include "DemoScene.h"
#include "NotificationManager.h"

#include <iostream>

PlayerInventory::PlayerInventory(Inventory* inv, Hotbar* hot)
{
	m_inventory = inv;
	m_hotbar = hot;
}



bool PlayerInventory::CanAdd(const std::string& ID, int amount)
{
	//-----------------------------WARNING WARNING-----------------------------
	//METHODE CODED WITH THE CUL:
	//IF(ADDITEM.MAKESNOISES == TRUE || REMOVEITEM.MAKESNOISES == TRUE) 
	//		REWRITE FUNCTION OR SOUNDS TOUT LE TEMPS ET CA VA ETRE CHIANTOS :/
	//-------------------------------------------------------------------------

	int remaining = amount;

	
	//C'est un peu degeu de faire comme ca...
	int hotbarTry = m_hotbar->AddItem(ID, remaining, false);
	remaining -= hotbarTry;

	if (hotbarTry > 0)
	{
		m_hotbar->RemoveItem(ID, hotbarTry, false);
	}


	int invTry = m_inventory->AddItem(ID, remaining, false);
	remaining -= invTry;

	if (invTry > 0)
	{
		m_inventory->RemoveItem(ID, invTry, false);
	}

	return remaining <= 0;
}

bool PlayerInventory::CanRemove(const std::string& ID, int amount)
{
	return CountItem(ID) >= amount;
}

int PlayerInventory::AddItem(const std::string& ID, int amount, bool requireFull)
{
	if (amount <= 0)
		return 0;

	if (requireFull && CanAdd(ID, amount) == false)
		return 0;


	int remaining = amount;

	int addedHotbar = m_hotbar->AddItem(ID, remaining, false);
	remaining -= addedHotbar;

	int addedInventory = m_inventory->AddItem(ID, remaining, false);
	remaining -= addedInventory;



	if (amount - remaining > 0 && m_notifManager)
	{
		ItemData* data = ItemDataBase::Get()->GetItem(ID);

		if (data)
		{
			m_notifManager->AddNotification("notificationBackground", data->GetPath(), data->GetName(), data->GetDescription(), std::to_string(amount - remaining));
		}
	}

	return amount - remaining;
}

int PlayerInventory::RemoveItem(const std::string& ID, int amount, bool requireFull)
{
	if (amount <= 0)
		return 0;

	if (requireFull && CanRemove(ID, amount) == false)
		return 0;


	int remaining = amount;

	int removedHotbar = m_hotbar->RemoveItem(ID, remaining, false);
	remaining -= removedHotbar;

	int removedInventory = m_inventory->RemoveItem(ID, remaining, false);
	remaining -= removedInventory;

	return amount - remaining;
}



int PlayerInventory::CountItem(const std::string& ID)
{
	return m_hotbar->CountItem(ID) + m_inventory->CountItem(ID);
}

bool PlayerInventory::HasItem(const std::string& ID)
{
	return CountItem(ID) > 0;
}

bool PlayerInventory::HasItems(const std::string& ID, int amount)
{
	return CountItem(ID) >= amount;
}

bool PlayerInventory::IsEmpty()
{
	for (InventorySlot* slot : m_hotbar->GetSlots())
	{
		if (slot->IsEmpty() == false)
			return false;
	}

	for (InventorySlot* slot : m_inventory->GetSlots())
	{
		if (slot->IsEmpty() == false)
			return false;
	}

	return true;
}



Inventory* PlayerInventory::GetInventory()
{
	return m_inventory;
}

Hotbar* PlayerInventory::GetHotbar()
{
	return m_hotbar;
}

int PlayerInventory::GetSelectedIndex()
{
	return m_hotbar->GetSelectedIndex();
}

ItemStack& PlayerInventory::GetSelectedStack()
{
	return m_hotbar->GetSelectedStack();
}

const std::string& PlayerInventory::GetSelectedItemID()
{
	return m_hotbar->GetSelectedItemID();
}

InventorySlot* PlayerInventory::GetInventorySlot(int index)
{
	return m_inventory->GetSlot(index);
}

InventorySlot* PlayerInventory::GetHotbarSlot(int index)
{
	return m_hotbar->GetSlot(index);
}



void PlayerInventory::Clear()
{
	for (InventorySlot* slot : m_hotbar->GetSlots())
		slot->Clear();

	for (InventorySlot* slot : m_inventory->GetSlots())
		slot->Clear();
}

void PlayerInventory::SetNotificationManager(NotificationManager* notificationManager)
{
	m_notifManager = notificationManager;
}