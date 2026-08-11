#include "pch.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "InventoryLogicManager.h"
#include "InventorySlot.h"

#include <iostream>

//void InventoryLogic::AddInventory(std::vector<InventorySlot*>& slots, uint8_t flags)
//{
//	std::cout << "inventory added\n";
//}
//
//void InventoryLogic::ClearInventories()
//{
//	std::cout << "inventory clear\n";
//}
//
//bool InventoryLogic::HasCursorItem()
//{
//	return false;
//}
//
//void InventoryLogic::PickupStack(InventorySlot* slot)
//{
//	std::cout << "\n\nPickupStack\n\n\n";
//}
//
//void InventoryLogic::PlaceStack(InventorySlot* slot)
//{
//	std::cout << "\n\nPlaceStack\n\n\n";
//}
//
//void InventoryLogic::SplitStack(InventorySlot* slot)
//{
//	std::cout << "\n\nSplitStack\n\n\n";
//}
//
//void InventoryLogic::MergeStacks(InventorySlot* slot)
//{
//	std::cout << "\n\nMergeStacks\n\n\n";
//}
//
//void InventoryLogic::SwapStacks(InventorySlot* slot)
//{
//	std::cout << "\n\nSwapStacks\n\n\n";
//}
//
//void InventoryLogic::PlaceSingleItem(InventorySlot* slot)
//{
//	std::cout << "\n\nPlaceSingleItem\n\n\n";
//}

void InventoryLogic::AddInventory(std::vector<InventorySlot*>& slots, uint8_t flags, InventoryType type)
{
	InventoryInfo inventory;

	inventory.flags = flags;
	inventory.slots = &slots;
	inventory.type = type;

	m_inventories.push_back(inventory);
}

void InventoryLogic::ClearInventories()
{
	m_inventories.clear();
}

ItemStack& InventoryLogic::GetCursorStack()
{
	return m_cursorStack;
}

bool InventoryLogic::HasCursorItem()
{
	return m_cursorStack.IsEmpty() == false;
}

void InventoryLogic::PickupStack(InventorySlot* slot)
{
	if (slot == nullptr || CanInteract(slot, PickItem) == false || slot->IsEmpty())
		return;

	m_cursorStack = slot->GetStack();
	slot->Clear();
}

void InventoryLogic::PlaceStack(InventorySlot* slot)
{
	if (slot == nullptr || m_cursorStack.IsEmpty())
		return;

	if (slot->IsEmpty())
	{
		if (CanInteract(slot, PlaceItem) == false)
			return;

		DoPlaceStacks(slot);
		return;
	}

	if (slot->GetStack().GetID() == m_cursorStack.GetID())
	{
		if (CanInteract(slot, MergeStack) == false)
			return;

		DoMergeStacks(slot);
		return;
	}

	if (CanInteract(slot, SwapItem) == false)
		return;

	DoSwapStacks(slot);
}

void InventoryLogic::SplitStack(InventorySlot* slot)
{
	if (slot == nullptr || CanInteract(slot, InventoryInterractionBit::SplitStack) == false || slot->IsEmpty())
		return;

	int amount = slot->GetStack().GetAmount();
	int hafl = amount / 2;

	if (hafl == 0)
		return;

	m_cursorStack = slot->GetStack();

	m_cursorStack.SetAmount(amount - hafl);

	slot->GetStack().SetAmount(hafl);
	slot->RefreshVisual();
}

void InventoryLogic::MergeStacks(InventorySlot* slot)
{
	if (slot == nullptr || CanInteract(slot, MergeStack) == false || slot->IsEmpty() || m_cursorStack.IsEmpty() 
		|| slot->GetStack().GetID() != m_cursorStack.GetID())
		return;

	DoMergeStacks(slot);
}

void InventoryLogic::SwapStacks(InventorySlot* slot)
{
	if (slot == nullptr || CanInteract(slot, SwapItem) == false)
		return;

	DoSwapStacks(slot);
}

void InventoryLogic::PlaceSingleItem(InventorySlot* slot)
{
	if (slot == nullptr || CanInteract(slot, PlaceItem) == false || m_cursorStack.IsEmpty())
		return;

	if (slot->IsEmpty())
	{
		ItemStack stack = m_cursorStack;

		stack.SetAmount(1);

		slot->SetStack(stack);

		m_cursorStack.SetAmount(m_cursorStack.GetAmount() - 1);
	}
	else
	{
		if (slot->GetStack().GetID() != m_cursorStack.GetID() || slot->GetStack().GetAmount() >= GetMaxStack(slot->GetStack().GetID()))
		{
			return;
		}

		slot->GetStack().SetAmount(slot->GetStack().GetAmount() + 1);
		slot->RefreshVisual();

		m_cursorStack.SetAmount(m_cursorStack.GetAmount() - 1);
	}

	if (m_cursorStack.GetAmount() == 0)
		m_cursorStack.Clear();
}

void InventoryLogic::BeginDrag()
{
	if (m_cursorStack.IsEmpty())
		return;

	m_dragSlots.clear();

	m_dragItemID = m_cursorStack.GetID();

	m_dragTotalAmount = m_cursorStack.GetAmount();
}

void InventoryLogic::AddDragSlot(InventorySlot* slot)
{
	if (slot == nullptr || CanInteract(slot, DragDistribute) == false)
		return;

	for (DraggedSlot& draggedSlot : m_dragSlots)
	{
		if (slot == draggedSlot.slot)
			return;

	}

	if (slot->IsEmpty() == false)
	{
		if (slot->GetStack().GetID() != m_dragItemID)
			return;

		if (slot->GetStack().GetAmount() >= GetMaxStack(m_dragItemID))
			return;
	}


	DraggedSlot drag;

	drag.slot = slot;
	drag.wasEmptyAtDragStart = slot->IsEmpty();
	drag.originalAmount = slot->GetStack().GetAmount();

	m_dragSlots.push_back(drag);
}

void InventoryLogic::CommitDrag() //oooooof
{
	if (m_dragSlots.empty() || m_cursorStack.IsEmpty())
	{
		CancelDrag();
		return;
	}

	int amount = m_cursorStack.GetAmount();
	int amountPerSlot = amount / m_dragSlots.size();


	if (amountPerSlot <= 0) //pas asserz d'items (on drag 6 slots et on n'a que 5 items par exemple)
	{
		CancelDrag();
		return;
	}


	for (DraggedSlot& dragged : m_dragSlots)
	{
		if (m_cursorStack.IsEmpty())
			break;

		InventorySlot* slot = dragged.slot;

		int maxStack = GetMaxStack(m_dragItemID);

		int currentAmount = slot->GetStack().GetAmount();

		int freeSpace = maxStack - currentAmount;

		int amountToPlace = std::min(amountPerSlot, freeSpace);

		if (amountToPlace <= 0)
			continue;

		if (dragged.wasEmptyAtDragStart)
		{
			slot->SetStack(ItemStack(m_dragItemID, amountToPlace));
		}
		else
		{
			slot->GetStack().SetAmount(currentAmount + amountToPlace);
			slot->RefreshVisual();
		}


		m_cursorStack.SetAmount(m_cursorStack.GetAmount() - amountToPlace);
	}


	if (m_cursorStack.GetAmount() <= 0)
		m_cursorStack.Clear();


	CancelDrag();
}

void InventoryLogic::CancelDrag()
{
	m_dragSlots.clear();

	m_dragItemID.clear();

	m_dragTotalAmount = 0;
}

void InventoryLogic::DropSlot(InventorySlot* slot)
{
	if (slot == nullptr || slot->IsEmpty() || CanInteract(slot, Drop) == false)
		return;

	slot->Clear();
}

void InventoryLogic::GatherAllItems(InventorySlot* slot)
{
	InventoryInfo* origin = FindInventory(slot);
	std::string itemID = m_cursorStack.GetID();
	int maxStack = GetMaxStack(itemID);

	if (origin == nullptr)
		return;

	//premier pasage on prends les rtestes
	for (InventoryInfo* inv : m_activeInventories)
	{
		if (inv->type != InventoryType::Basic)
			continue;


		for (InventorySlot* slot : *inv->slots)
		{
			if (m_cursorStack.GetAmount() >= maxStack)
				return;

			if (slot->IsEmpty() || slot->GetStack().GetID() != m_cursorStack.GetID())
				continue;

			if (slot->GetStack().GetAmount() >= maxStack)
				continue;

			int space = maxStack - m_cursorStack.GetAmount();
			int moved = std::min(space, slot->GetStack().GetAmount());

			m_cursorStack.SetAmount(m_cursorStack.GetAmount() + moved);

			slot->GetStack().SetAmount(slot->GetStack().GetAmount() - moved);
			slot->RefreshVisual();

			if (slot->GetStack().GetAmount() <= 0)
				slot->Clear();
		}
	}


	for (InventoryInfo* inv : m_activeInventories)
	{
		if (inv->type != InventoryType::PlayerInventory)
			continue;


		for (InventorySlot* slot : *inv->slots)
		{
			if (m_cursorStack.GetAmount() >= maxStack)
				return;

			if (slot->IsEmpty() || slot->GetStack().GetID() != m_cursorStack.GetID())
				continue;

			if (slot->GetStack().GetAmount() >= maxStack)
				continue;

			int space = maxStack - m_cursorStack.GetAmount();
			int moved = std::min(space, slot->GetStack().GetAmount());

			m_cursorStack.SetAmount(m_cursorStack.GetAmount() + moved);

			slot->GetStack().SetAmount(slot->GetStack().GetAmount() - moved);
			slot->RefreshVisual();

			if (slot->GetStack().GetAmount() <= 0)
				slot->Clear();
		}
	}


	for (InventoryInfo* inv : m_activeInventories)
	{
		if (inv->type != InventoryType::Hotbar)
			continue;


		for (InventorySlot* slot : *inv->slots)
		{
			if (m_cursorStack.GetAmount() >= maxStack)
				return;

			if (slot->IsEmpty() || slot->GetStack().GetID() != m_cursorStack.GetID())
				continue;

			if (slot->GetStack().GetAmount() >= maxStack)
				continue;

			int space = maxStack - m_cursorStack.GetAmount();
			int moved = std::min(space, slot->GetStack().GetAmount());

			m_cursorStack.SetAmount(m_cursorStack.GetAmount() + moved);

			slot->GetStack().SetAmount(slot->GetStack().GetAmount() - moved);
			slot->RefreshVisual();

			if (slot->GetStack().GetAmount() <= 0)
				slot->Clear();
		}
	}




	//second passage prendre les enters
	for (InventoryInfo* inv : m_activeInventories)
	{
		if (inv->type != InventoryType::Basic)
			continue;


		for (InventorySlot* slot : *inv->slots)
		{
			if (m_cursorStack.GetAmount() >= maxStack)
				return;

			if (slot->IsEmpty() || slot->GetStack().GetID() != m_cursorStack.GetID())
				continue;

			int space = maxStack - m_cursorStack.GetAmount();
			int moved = std::min(space, slot->GetStack().GetAmount());

			m_cursorStack.SetAmount(m_cursorStack.GetAmount() + moved);

			slot->GetStack().SetAmount(slot->GetStack().GetAmount() - moved);
			slot->RefreshVisual();

			if (slot->GetStack().GetAmount() <= 0)
				slot->Clear();
		}
	}


	for (InventoryInfo* inv : m_activeInventories)
	{
		if (inv->type != InventoryType::PlayerInventory)
			continue;


		for (InventorySlot* slot : *inv->slots)
		{
			if (m_cursorStack.GetAmount() >= maxStack)
				return;

			if (slot->IsEmpty() || slot->GetStack().GetID() != m_cursorStack.GetID())
				continue;

			int space = maxStack - m_cursorStack.GetAmount();
			int moved = std::min(space, slot->GetStack().GetAmount());

			m_cursorStack.SetAmount(m_cursorStack.GetAmount() + moved);

			slot->GetStack().SetAmount(slot->GetStack().GetAmount() - moved);
			slot->RefreshVisual();

			if (slot->GetStack().GetAmount() <= 0)
				slot->Clear();
		}
	}


	for (InventoryInfo* inv : m_activeInventories)
	{
		if (inv->type != InventoryType::Hotbar)
			continue;


		for (InventorySlot* slot : *inv->slots)
		{
			if (m_cursorStack.GetAmount() >= maxStack)
				return;

			if (slot->IsEmpty() || slot->GetStack().GetID() != m_cursorStack.GetID())
				continue;

			int space = maxStack - m_cursorStack.GetAmount();
			int moved = std::min(space, slot->GetStack().GetAmount());

			m_cursorStack.SetAmount(m_cursorStack.GetAmount() + moved);

			slot->GetStack().SetAmount(slot->GetStack().GetAmount() - moved);
			slot->RefreshVisual();

			if (slot->GetStack().GetAmount() <= 0)
				slot->Clear();
		}
	}
}

void InventoryLogic::ActivateInventory(std::vector<InventorySlot*>& slots)
{
	for (InventoryInfo& inv : m_inventories)
	{
		if (inv.slots == &slots)
		{
			m_activeInventories.push_back(&inv);
			return;
		}
	}
}

void InventoryLogic::DisactivateInventory(std::vector<InventorySlot*>& slots)
{
	for (int i = 0; i < m_activeInventories.size(); i++)
	{
		if (m_activeInventories[i]->slots == &slots)
		{
			m_activeInventories.erase(m_activeInventories.begin() + i);
			return;
		}
	}
}

bool InventoryLogic::HasFlag(uint8_t flags, InventoryInterractionBit flag)
{
	return (flags & flag) != 0;
}

InventoryLogic::InventoryInfo* InventoryLogic::FindInventory(InventorySlot* slot)
{
		for (InventoryInfo& inv : m_inventories)
		{
			for (InventorySlot* s : *inv.slots)
			{
				if (s == slot)
					return &inv;
			}
		}
	
		return nullptr;
}

bool InventoryLogic::CanInteract(InventorySlot* slot, InventoryInterractionBit flag)
{
	if (slot == nullptr)
		return false;

	InventoryInfo* inv = FindInventory(slot);

	if (inv == nullptr)
		return false;

	return HasFlag(inv->flags, flag);
}

int InventoryLogic::GetMaxStack(const std::string& id)
{
	ItemData* item = ItemDataBase::Get()->GetItem(id);

	if (item == nullptr)
		return 1;

	return item->GetMaxStack();
}

void InventoryLogic::DoPlaceStacks(InventorySlot* slot)
{
	slot->SetStack(m_cursorStack);

	m_cursorStack.Clear();
}

void InventoryLogic::DoMergeStacks(InventorySlot* slot)
{
	int max = GetMaxStack(slot->GetStack().GetID());

	int currentAmount = slot->GetStack().GetAmount();

	int freeSpace = max - currentAmount;

	int cursorAmount = m_cursorStack.GetAmount();

	if (freeSpace <= 0)
		return;

	int moved = std::min(freeSpace, cursorAmount);

	slot->GetStack().SetAmount(currentAmount + moved);

	slot->RefreshVisual();

	m_cursorStack.SetAmount(cursorAmount - moved);

	if (m_cursorStack.GetAmount() == 0)
		m_cursorStack.Clear();
}

void InventoryLogic::DoSwapStacks(InventorySlot* slot)
{
	ItemStack a = m_cursorStack;

	m_cursorStack = slot->GetStack();

	slot->SetStack(a);
}