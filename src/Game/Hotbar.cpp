#include "pch.h"
#include "Hotbar.h"
#include "ItemDataBase.h"

#include <cassert>


Hotbar::Hotbar(int slotcount, int invSlotCount)
{
	m_slotCount = slotcount;
	m_invSlotCount = invSlotCount;
}



void Hotbar::OnInitialize()
{
	SetDrawLayer(100);

	const float spacing = 60.f;

	float startX = GetPosition({ 0, 0 }).GetX() + 48.f;
	float startY = GetPosition({ 0, 0 }).GetY() + 45.f;

	for (int i = 0; i < m_slotCount; i++)
	{
		int row = i / 10;
		int col = i % 10;

		Vector2f pos = { startX + col * spacing, startY + row * spacing };

		InventorySlot* slot = GetScene()->CreateButton<InventorySlot>(pos, { 54,54 }, "InventorySlot", "slot_" + std::to_string(i + m_invSlotCount) + "_background", "", true);

		slot->InitText("slot_" + std::to_string(i + m_invSlotCount) + "_amount_text", "");

		slot->SetDrawLayer(101);

		m_slots.push_back(slot);
	}
}



void Hotbar::OnUpdate()
{
	for (int i = 0; i < m_slots.size(); i++)//FIX multisprite
	{
		if (i == m_selectedIndex)
			m_slots[i]->SetSprite("InventorySlotHover");
		else
			m_slots[i]->SetSprite("InventorySlot"); 
	}
}



bool Hotbar::CanAdd(ItemID ID, int amount)
{
	ItemData* data = ItemDataBase::Get()->GetItem(ID);

	if (data == nullptr)
		return false;

	int remaining = amount;

	for (InventorySlot* slot : m_slots)
	{
		if (slot->IsEmpty() == false)
		{
			ItemStack& stack = slot->GetStack();

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



bool Hotbar::CanRemove(ItemID ID, int amount)
{
	return CountItem(ID) >= amount;
}



int Hotbar::AddItem(ItemID ID, int amount, bool requireFull)
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
	for (InventorySlot* slot : m_slots)
	{
		if (slot->IsEmpty())
			continue;

		ItemStack& stack = slot->GetStack();

		if (stack.GetID() != ID)
			continue;

		int freeSpace = data->GetMaxStack() - stack.GetAmount();
		int toAdd = std::min(freeSpace, remaining);

		stack.SetAmount(stack.GetAmount() + toAdd);
		slot->RefreshVisual();
		remaining -= toAdd;

		if (remaining <= 0)
			return amount;
	}


	//remplir les slots vies
	for (InventorySlot* slot : m_slots)
	{
		if (slot->IsEmpty() == false)
			continue;

		int toAdd = std::min(data->GetMaxStack(), remaining);

		slot->SetStack(ItemStack(ID, toAdd));
		remaining -= toAdd;

		if (remaining <= 0)
			return amount;
	}

	return amount - remaining;
}



int Hotbar::RemoveItem(ItemID ID, int amount, bool requireFull)
{
	/*

		Meme principe que addItem mais en retirant les items

	*/

	if (amount <= 0)
		return 0;

	if (requireFull && CanRemove(ID, amount) == false)
		return 0;

	int remaining = amount;

	for (InventorySlot* slot : m_slots)
	{
		if (slot->IsEmpty())
			continue;

		ItemStack& stack = slot->GetStack();

		if (stack.GetID() != ID)
			continue;

		int available = stack.GetAmount();
		int toRemove = std::min(available, remaining);

		stack.SetAmount(available - toRemove);
		slot->RefreshVisual();

		if (stack.GetAmount() <= 0)
			slot->Clear();

		remaining -= toRemove;

		if (remaining <= 0)
			break;
	}

	return amount - remaining;
}	



int Hotbar::CountItem(ItemID ID)
{
	int total = 0;

	for (InventorySlot* slot : m_slots)
	{
		if (slot->IsEmpty())
			continue;

		if (slot->GetStack().GetID() == ID)
			total += slot->GetStack().GetAmount();
	}

	return total;
}



InventorySlot* Hotbar::GetSlot(int index)
{
	assert(index >= 0 && index < m_slots.size());
	return m_slots[index];
}



std::vector<InventorySlot*>& Hotbar::GetSlots()
{
	return m_slots;
}



void Hotbar::Select(int index)
{
	if (index < 0 || index >= m_slotCount)
		return;

	m_selectedIndex = index;
}



void Hotbar::GoNext(int dir)
{
	m_selectedIndex += dir;

	if (m_selectedIndex >= m_slotCount)
		m_selectedIndex = 0;

	if (m_selectedIndex < 0) 
		m_selectedIndex = m_slotCount - 1;
}



int Hotbar::GetSelectedIndex()
{
	return m_selectedIndex;
}



ItemStack& Hotbar::GetSelectedStack()
{
	return m_slots[m_selectedIndex]->GetStack();
}



ItemID Hotbar::GetSelectedItemID()
{
	auto& stack = GetSelectedStack();

	if (stack.IsEmpty())
		return ITEM_NOTHING;
	
	return stack.GetID();
}