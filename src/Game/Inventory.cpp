#include "pch.h"
#include "Inventory.h"
#include "ItemDataBase.h"
#include "ItemStack.h"

#include <cassert>

Inventory::Inventory(int slotCount)
{
	m_slots.resize(slotCount);
}



bool Inventory::CanAdd(ItemID ID, int amount)
{
	ItemData* data = ItemDataBase::Get()->GetItem(ID);

	if (data == nullptr)
		return false;

	int remaining = amount;

	for (InventorySlot& slot : m_slots)
	{
		if (slot.IsEmpty() == false)
		{
			ItemStack& stack = slot.GetStack();

			if (stack.GetID() != ID)
				continue;

			int freeSpace = data->GetMaxStack() - stack.GetAmount();
			remaining -= std::min(freeSpace, remaining);
		}
		else
		{
			remaining -= std::min(data->GetMaxStack(), remaining);
		}

		if (remaining <= 0)
			return true;
	}
	return false;
}

bool Inventory::CanRemove(ItemID ID, int amount)
{
	return CountItem(ID) >= amount;
}

int Inventory::AddItem(ItemID ID, int amount, bool requireFull)
{
	/*
	
	requireFull :
	true = on ajoute uniquement si TOUT rentre dans l'inventaire
	false =  on ajoute le plus possible meme si tout ne rentre pas 

	retroune le d'item ajoutes
	
	*/


	if (amount <= 0)
		return 0;

	ItemData* data = ItemDataBase::Get()->GetItem(ID);

	if (data == nullptr)
		return 0;

	if (requireFull && CanAdd(ID, amount) == false)
		return 0;

	int remaining = amount;


	//remplir les stacks existantes
	for (InventorySlot& slot : m_slots)
	{
		if (slot.IsEmpty())
			continue;

		ItemStack& stack = slot.GetStack();

		if (stack.GetID() != ID)
			continue;

		int freeSpace = data->GetMaxStack() - stack.GetAmount();
		int toAdd = std::min(freeSpace, remaining);

		stack.SetAmount(stack.GetAmount() + toAdd);
		remaining -= toAdd;

		if (remaining <= 0)
			return amount;
	}


	//remplir les slots vies
	for (InventorySlot& slot : m_slots)
	{
		if (slot.IsEmpty() == false)
			continue;

		int toAdd = std::min(data->GetMaxStack(), remaining);

		slot.SetStack(ItemStack(ID, toAdd));
		remaining -= toAdd;

		if (remaining <= 0)
			return amount;
	}


	return amount - remaining;
}



int Inventory::RemoveItem(ItemID ID, int amount, bool requireFull)
{
	/*
	
	Meme principe que addItem mais en retirant les items
		
	*/
	
	if (amount <= 0)
		return 0;

	if (requireFull && CanRemove(ID, amount) == false)
		return 0;

	int remaining = amount;

	for (InventorySlot& slot : m_slots)
	{
		if (slot.IsEmpty())
			continue;

		ItemStack& stack = slot.GetStack();

		if (stack.GetID() != ID)
			continue;

		int available = stack.GetAmount();
		int toRemove = std::min(available, remaining);

		stack.SetAmount(available - toRemove);

		if (stack.GetAmount() <= 0)
			slot.Clear();

		remaining -= toRemove;

		if (remaining <= 0)
			break;
	}

	return amount - remaining;
}



int Inventory::CountItem(ItemID ID)
{
	int total = 0;

	for (InventorySlot& slot : m_slots)
	{
		if (slot.IsEmpty())
			continue;

		if (slot.GetStack().GetID() == ID)
			total += slot.GetStack().GetAmount();
	}

	return total;
}

InventorySlot& Inventory::GetSlot(int index)
{
	assert(index >= 0 && index < m_slots.size());
	return m_slots[index];
}

std::vector<InventorySlot>& Inventory::GetSlots()
{
	return m_slots;
}

int Inventory::GetSlotCount()
{
	return (int)m_slots.size();
}