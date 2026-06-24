#include "pch.h"
#include "InventoryInteractionManager.h"
#include "ItemData.h"
#include "ItemDataBase.h"

#include <iostream>


void InventoryManager::AddInventory(std::vector<InventorySlot*>& slots, uint8_t bit)
{
	InventoryWithFlag inventory;

	inventory.m_slots = &slots;
	inventory.m_flags = bit;

	m_iventoriesWithFlag.push_back(inventory);

	for (InventorySlot* slot : slots)
	{
		slot->SetOnLeftClick([this, slot]() { HandleLeftClick(slot); });

		slot->SetOnRightClick([this, slot]() { HandleRightClick(slot); });

		slot->SetOnDoubleLeftClick([this, slot]() { HandleDoubleLeftClick(slot); });

		slot->SetOnHeldLeftClick([this, slot]() { HandleLeftHeld(slot); });

		slot->SetOnReleaseLeftClick([this]() { HandleLeftRelease(); });
	}

}

void InventoryManager::ClearInventories()
{
	m_iventoriesWithFlag.clear();
}

ItemStack& InventoryManager::GetCursorStack()
{
	return m_cursorStack;
}

bool InventoryManager::HasCursorItem()
{
	return m_cursorStack.IsEmpty() == false;
}

bool InventoryManager::HasFlag(uint8_t flags, InventoryInterractionBit flag)
{
	return ((flags & flag) != 0);
}

InventoryManager::InventoryWithFlag* InventoryManager::FindInventory(InventorySlot* slot)
{
	for (InventoryWithFlag& inventory : m_iventoriesWithFlag)
	{
		for (InventorySlot* currentTestedSlot : *inventory.m_slots)
		{
			if (currentTestedSlot == slot)
			{
				return &inventory;
			}
		}
	}

	return nullptr;
}

void InventoryManager::HandleLeftClick(InventorySlot* slot)
{
	if (slot == nullptr)
		return;



	if (HasCursorItem() == false)
	{
		if (slot->IsEmpty())
			return;

		PickupStack(slot);
		return;
	}

	
	
	if (slot->IsEmpty())
	{
		PlaceStack(slot);
		return;

	}



	if (slot->GetStack().GetID() == m_cursorStack.GetID())
	{
		MergeStacks(slot);
		return;
	}



	SwapStacks(slot);
}

void InventoryManager::HandleRightClick(InventorySlot* slot)
{
	if (slot == nullptr)
		return;



	if (HasCursorItem())
	{
		PlaceSingleItem(slot);
		return;
	}



	if (slot->IsEmpty())
	{
		return;
	}



	SplitStack(slot);
}

void InventoryManager::HandleDoubleLeftClick(InventorySlot* slot)
{
	GatherAllStacks(slot);
}

void InventoryManager::HandleDropOutside()
{
	//Voir avec Nathan

	m_cursorStack.Clear(); //En attendatn
}

void InventoryManager::HandleLeftHeld(InventorySlot* slot)
{
	if (slot == nullptr /*|| HasCursorItem() == false*/ || CanInteract(slot, InventoryInterractionBit::DragDistribute) == false)
		return;

	if (m_isDragging == false)
	{
		m_isDragging = true;
		return;
	}

	for (InventorySlot* current : m_alreadyDraggedSlots)
	{
		if (current == slot)
			return;
	}

	m_alreadyDraggedSlots.push_back(slot);

	PlaceSingleItem(slot);
}

void InventoryManager::HandleLeftRelease()
{
	m_isDragging = false;

	m_alreadyDraggedSlots.clear();
}

void InventoryManager::PickupStack(InventorySlot* slot)
{
	if (CanInteract(slot, InventoryInterractionBit::PickItem) == false || slot->IsEmpty())
		return;

	m_cursorStack = slot->GetStack();
	slot->Clear();
}

void InventoryManager::PlaceStack(InventorySlot* slot)
{
	if (CanInteract(slot, InventoryInterractionBit::PlaceItem) == false || slot->IsEmpty() == false || HasCursorItem() == false)
		return;

	slot->SetStack(m_cursorStack);
	m_cursorStack.Clear();
}

void InventoryManager::SwapStacks(InventorySlot* slot)
{
	if (CanInteract(slot, InventoryInterractionBit::SwapItem) == false || HasCursorItem() == false)
		return;

	ItemStack intermediaire = slot->GetStack();
	slot->SetStack(m_cursorStack);
	m_cursorStack = intermediaire;
}

void InventoryManager::MergeStacks(InventorySlot* slot)
{
	if (CanInteract(slot, InventoryInterractionBit::MergeStack) == false || slot->IsEmpty() || HasCursorItem() == false)
		return;

	if (slot->GetStack().GetID() != m_cursorStack.GetID())
		return;

	int max = GetMaxStack(slot->GetStack().GetID());
	if (max <= 0)
		return;

	int space = max - slot->GetStack().GetAmount();
	if (space <= 0)
		return;

	int toMove = std::min(space, m_cursorStack.GetAmount());
	slot->GetStack().SetAmount(slot->GetStack().GetAmount() + toMove); //Clean avec des var
	m_cursorStack.SetAmount(m_cursorStack.GetAmount() - toMove); //Idem

	if (m_cursorStack.GetAmount() <= 0)
		m_cursorStack.Clear();

	slot->RefreshVisual();
}

void InventoryManager::SplitStack(InventorySlot* slot)
{
	if (CanInteract(slot, InventoryInterractionBit::SplitStack) == false || slot->IsEmpty())
		return;

	
	
	if (slot->GetStack().GetAmount() <= 1)
		return;

	int half = (slot->GetStack().GetAmount() + 1) / 2;

	m_cursorStack = slot->GetStack();
	m_cursorStack.SetAmount(half);

	slot->GetStack().SetAmount(slot->GetStack().GetAmount() - half);

	if (slot->GetStack().GetAmount() <= 0)
		slot->Clear();
	else
		slot->RefreshVisual();
}

void InventoryManager::PlaceSingleItem(InventorySlot* slot)
{
	if (CanInteract(slot, InventoryInterractionBit::PlaceItem) == false || HasCursorItem() == false)
		return;

	int max = GetMaxStack(m_cursorStack.GetID());
	if (max <= 0)
		return;

	if (slot->IsEmpty())
	{
		ItemStack stack;
		stack.SetID(m_cursorStack.GetID());
		stack.SetAmount(1);

		slot->SetStack(stack);

		m_cursorStack.SetAmount(m_cursorStack.GetAmount() - 1);

		if (m_cursorStack.GetAmount() <= 0)
			m_cursorStack.Clear();

		slot->RefreshVisual();

		return;

	}


	if (slot->GetStack().GetID() != m_cursorStack.GetID() || slot->GetStack().GetAmount() >= max)
		return;
	

	slot->GetStack().SetAmount(slot->GetStack().GetAmount() + 1);

	m_cursorStack.SetAmount(m_cursorStack.GetAmount() - 1);

	if (m_cursorStack.GetAmount() <= 0)
		m_cursorStack.Clear();

	slot->RefreshVisual();

}

void InventoryManager::GatherAllStacks(InventorySlot* slot)
{
	if (CanInteract(slot, InventoryInterractionBit::DoubleClickGather) == false || slot->IsEmpty())
		return;

	std::string id = slot->GetStack().GetID();
	int max = GetMaxStack(id);

	if (max <= 0)
		return;

	ItemStack& base = slot->GetStack();


	for (InventoryWithFlag& inv : m_iventoriesWithFlag)
	{
		if (HasFlag(inv.m_flags, InventoryInterractionBit::DoubleClickGather) == false)
			continue;

		for (InventorySlot* s : *inv.m_slots)
		{
			if (s == nullptr || s == slot || s->IsEmpty() || s->GetStack().GetID() != id)
				continue;

			int space = max - base.GetAmount();
			if (space <= 0)
				return;

			int move = std::min(space, s->GetStack().GetAmount());

			base.SetAmount(base.GetAmount() + move);

			s->GetStack().SetAmount(s->GetStack().GetAmount() - move);

			if (s->GetStack().GetAmount() <= 0)
				s->Clear();

			s->RefreshVisual();
		}
	}
	slot->RefreshVisual();
}

int InventoryManager::GetMaxStack(const std::string& itemID)
{
	ItemData* data = ItemDataBase::Get()->GetItem(itemID);

	if (data == nullptr)
		return -1;
	
	return data->GetMaxStack();
}

bool InventoryManager::CanInteract(InventorySlot* slot, InventoryInterractionBit flag)
{
	if (slot == nullptr)
		return false;



	InventoryWithFlag* inventory = FindInventory(slot);

	if (inventory == nullptr)
		return false;

	return HasFlag(inventory->m_flags, flag);
}
