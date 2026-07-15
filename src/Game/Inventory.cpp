#include "pch.h"
#include "Inventory.h"
#include "ItemDataBase.h"
#include "ItemStack.h"
#include "SpriteContainer.h"
#include <cassert>

const int columns = 10;

const float spacing = 60.f;

Inventory::Inventory(int slotCount)
{
	m_slotCount = slotCount;
}

void Inventory::OnInitialize()
{
	SetDrawLayer(INT_MAX - 2);

	float startX = GetPosition({ 0, 0 }).GetX() + 48.f;
	float startY = GetPosition({ 0, 0 }).GetY() + 45.f;

	for (int i = 0; i < m_slotCount; i++)
	{
		int row = i / columns;
		int col = i % columns;

		Vector2f pos = { startX + col * spacing, startY + row * spacing };

		InventorySlot* slot = GetScene()->CreateButton<InventorySlot>(pos, { 54,54 }, "InventorySlot", "slot_" + std::to_string(i) + "_background", "", true);
		slot->GetCollider()->SetIsTrigger(true);

		slot->AddSprite("item_texture", "", { 54, 54 }, 1, { 0.0f, 0.0f }, true);

		slot->InitText("slot_" + std::to_string(i) + "_amount_text", "");

		m_slots.push_back(slot);
	}
	GetCollider()->SetIsTrigger(true);

}

bool Inventory::CanAdd(const std::string& ID, int amount)
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

bool Inventory::CanRemove(const std::string& ID, int amount)
{
	return CountItem(ID) >= amount;
}

int Inventory::AddItem(const std::string& ID, int amount, bool requireFull)
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

int Inventory::RemoveItem(const std::string& ID, int amount, bool requireFull)
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

int Inventory::CountItem(const std::string& ID)
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

void Inventory::SetVisible(bool visible)
{
	SetIsVisible(visible);

	for (InventorySlot* slot : m_slots)
	{
		slot->SetIsVisible(visible);
		slot->RefreshVisual();
	}
}

void Inventory::SetScene(Scene* p_newScene)
{
	m_scene = p_newScene;

	for (InventorySlot* s : m_slots)
	{
		s->SetScene(p_newScene);
	}

}

InventorySlot* Inventory::GetSlot(int index)
{
	assert(index >= 0 && index < static_cast<int>(m_slots.size()));
	return m_slots[index];
}

std::vector<InventorySlot*>& Inventory::GetSlots()
{
	return m_slots;
}

int Inventory::GetSlotCount()
{
	return (int)m_slots.size();
}

void Inventory::SetPosition(const Vector2f& p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);
	
	float startX = GetPosition({ 0, 0 }).GetX() + 48.f;
	float startY = GetPosition({ 0, 0 }).GetY() + 45.f;

	for (int i = 0; i < m_slotCount; i++)
	{
		int row = i / columns;
		int col = i % columns;

		Vector2f pos = { startX + col * spacing, startY + row * spacing };

		m_slots[i]->SetPosition(pos);
	}
}

bool Inventory::HasAmountOfItem(const std::string& p_ID, int p_amount)
{
	ItemData* data = ItemDataBase::Get()->GetItem(p_ID);

	if (data == nullptr)
		return false;

	int amount = 0;

	for (InventorySlot* slot : m_slots)
	{
		if (slot->IsEmpty())
			continue;

		ItemStack item = slot->GetStack();

		if (item.GetID() == p_ID)
			amount += item.GetAmount();
	}

	if (amount >= p_amount)
		return true;
}
