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
		slot->SetOnLeftClick([this, slot]() { OnLeftDown(slot); });
		slot->SetOnRightClick([this, slot]() { OnRightDown(slot); });
		slot->SetOnDoubleLeftClick([this, slot]() { OnDoubleLeftClick(slot); });

		slot->SetOnLeftHeldEnter([this, slot]() { OnLeftHeldEnter(slot); });
		slot->SetOnRightHeldEnter([this, slot]() { OnRightHeldEnter(slot); });

		slot->SetOnLeftRelease([this, slot]() { OnLeftRelease(slot); });
		slot->SetOnRightRelease([this, slot]() { OnRightRelease(slot); });
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

void InventoryManager::OnLeftDown(InventorySlot* slot)
{
	if (slot == nullptr)
		return;

	m_pressedSlot = slot;

	if (HasCursorItem() == false)
	{
		if (slot->IsEmpty())
			return;

		PickupStack(slot);

		auto now = std::chrono::steady_clock::now();
		float elapsed = std::chrono::duration<float>(now - m_lastPickupTime).count();
		bool isDoubleClick = (elapsed <= m_doubleClickDelay && m_lastPickedSlot == slot);
		m_lastPickupTime = now;
		m_lastPickedSlot = slot;

		if (isDoubleClick)
			OnDoubleLeftClick(slot);

		m_pressedSlot = nullptr;

		m_state = State::Holding;
		return;
	}

	if (slot->IsEmpty() == false)
	{
		m_state = State::Holding;
		return;
	}

	m_state = State::LeftPendingPlace;
	m_dragSlots.clear();
	m_dragOriginalAmount.clear();
	m_dragTotalAtStart = m_cursorStack.GetAmount();
}

void InventoryManager::OnRightDown(InventorySlot* slot)
{
	if (slot == nullptr)
		return;

	m_pressedSlot = slot;

	if (HasCursorItem() == false)
	{
		if (slot->IsEmpty())
			return;

		SplitStackToCursor(slot);

		m_pressedSlot = nullptr;

		m_state = State::Holding;
		return;
	}

	if (slot->IsEmpty() == false)
	{
		m_state = State::Holding;
		return;
	}

	m_state = State::RightPendingPlace;
	m_dragSlots.clear();
	m_dragOriginalAmount.clear();
	m_dragTotalAtStart = m_cursorStack.GetAmount();
}

void InventoryManager::OnLeftHeldEnter(InventorySlot* slot)
{
	if (slot == nullptr /*|| HasCursorItem() == false*/)
	{
		std::cout << "caca" << std::endl;
		return;
	}

	if (m_state == State::LeftPendingPlace)
	{
		if (HasCursorItem() == false)
			return;

		m_state = State::LeftDragSplit;
		m_dragSlots.clear();
		m_dragOriginalAmount.clear();
		m_dragItemID = m_cursorStack.GetID();
		m_dragTotalAtStart = m_cursorStack.GetAmount();
	}

	if (m_state == State::LeftDragSplit)
	{
		for (InventorySlot* s : m_dragSlots)
		{
			if (s == slot)
				return;
		}

		if (CanInteract(slot, DragDistribute) == false)
			return;

		if (slot->IsEmpty() == false)
		{
			if (slot->GetStack().GetID() != m_dragItemID)
				return;

			int max = GetMaxStack(m_dragItemID);

			if (max > 0 && slot->GetStack().GetAmount() >= max)
				return;
		}

		int originalAmount = slot->IsEmpty() ? 0 : slot->GetStack().GetAmount();
		m_dragSlots.push_back(slot);
		m_dragOriginalAmount.push_back(originalAmount);
		RedistributeDrag();
	}
}

void InventoryManager::OnRightHeldEnter(InventorySlot* slot)
{
	if (slot == nullptr || HasCursorItem() == false)
		return;

	if (m_state == State::RightPendingPlace)
	{
		m_state = State::RightDragSplit;
		m_dragSlots.clear();
		m_dragOriginalAmount.clear();
		m_dragTotalAtStart = m_cursorStack.GetAmount();

		if (m_pressedSlot != nullptr && m_cursorStack.GetAmount() > 0)
		{
			if (CanInteract(m_pressedSlot, DragDistribute))
			{
				m_dragSlots.push_back(m_pressedSlot);
				PlaceSingleItem(m_pressedSlot);
			}
		}
	}

	if (m_state == State::RightDragSplit)
	{
		for (InventorySlot* s : m_dragSlots)
		{
			if (s == slot)
				return;
		}

		if (CanInteract(slot, DragDistribute) == false)
			return;

		if (slot->IsEmpty() == false)
		{
			if (slot->GetStack().GetID() != m_cursorStack.GetID())
				return;

			int max = GetMaxStack(m_cursorStack.GetID());

			if (max > 0 && slot->GetStack().GetAmount() >= max)
				return;
		}

		if (m_cursorStack.GetAmount() <= 0)
			return;

		m_dragSlots.push_back(slot);
		PlaceSingleItem(slot);
	}
}

void InventoryManager::OnLeftRelease(InventorySlot* slot)
{
	static bool s_releaseHandled = false;

	if (s_releaseHandled)
		return;

	s_releaseHandled = true;

	switch (m_state)
	{
	case State::LeftPendingPlace:
	{
		InventorySlot* target = m_pressedSlot;

		if (target != nullptr && HasCursorItem())
		{
			if (target->IsEmpty())
			{
				if (CanInteract(target, PlaceItem))
					PlaceFullStack(target);
			}
			else if (target->GetStack().GetID() == m_cursorStack.GetID())
			{
				if (CanInteract(target, MergeStack))
					MergeStacks(target);
			}
			else
			{
				if (CanInteract(target, SwapItem))
					SwapStacks(target);
			}
		}

		m_state = HasCursorItem() ? State::Holding : State::Idle;
		break;
	}

	case State::LeftDragSplit:
	{
		CommitDrag();
		m_state = HasCursorItem() ? State::Holding : State::Idle;
		break;
	}

	case State::Holding:
	{
		InventorySlot* target = m_pressedSlot;

		if (target != nullptr && HasCursorItem())
		{
			if (target->IsEmpty())
			{
				if (CanInteract(target, PlaceItem))
					PlaceFullStack(target);
			}
			else if (target->GetStack().GetID() == m_cursorStack.GetID())
			{
				if (CanInteract(target, MergeStack))
					MergeStacks(target);
			}
			else
			{
				if (CanInteract(target, SwapItem))
					SwapStacks(target);
			}
		}

		m_state = HasCursorItem() ? State::Holding : State::Idle;
		break;
	}

	default:
		break;
	}

	m_pressedSlot = nullptr;
	m_dragSlots.clear();
	m_dragOriginalAmount.clear();

	s_releaseHandled = false;
}

void InventoryManager::OnRightRelease(InventorySlot* slot)
{
	static bool s_releaseHandled = false;

	if (s_releaseHandled)
		return;

	s_releaseHandled = true;

	switch (m_state)
	{
	case State::RightPendingPlace:
	{
		InventorySlot* target = m_pressedSlot;

		if (target != nullptr && HasCursorItem())
		{
			if (target->IsEmpty())
			{
				if (CanInteract(target, PlaceItem))
					PlaceSingleItem(target);
			}
			else if (target->GetStack().GetID() == m_cursorStack.GetID())
			{
				if (CanInteract(target, MergeStack))
					PlaceSingleItem(target);
			}
			else
			{
				if (CanInteract(target, SwapItem))
					SwapStacks(target);
			}
		}

		m_state = HasCursorItem() ? State::Holding : State::Idle;
		break;
	}

	case State::RightDragSplit:
	{
		m_state = HasCursorItem() ? State::Holding : State::Idle;
		break;
	}

	case State::Holding:
	{
		InventorySlot* target = m_pressedSlot;

		if (target != nullptr && HasCursorItem())
		{
			if (target->IsEmpty())
			{
				if (CanInteract(target, PlaceItem))
					PlaceSingleItem(target);
			}
			else if (target->GetStack().GetID() == m_cursorStack.GetID())
			{
				if (CanInteract(target, MergeStack))
					PlaceSingleItem(target);
			}
			else
			{
				if (CanInteract(target, SwapItem))
					SwapStacks(target);
			}
		}

		m_state = HasCursorItem() ? State::Holding : State::Idle;
		break;
	}

	default:
		break;
	}

	m_pressedSlot = nullptr;
	m_dragSlots.clear();
	m_dragOriginalAmount.clear();

	s_releaseHandled = false;
}

void InventoryManager::OnDoubleLeftClick(InventorySlot* slot)
{
	if (CanInteract(slot, DoubleClickGather) == false)
		return;

	if (HasCursorItem() == false)
		return;

	std::string id = m_cursorStack.GetID();
	int max = GetMaxStack(id);

	if (max <= 0)
		return;

	for (InventoryWithFlag& inv : m_iventoriesWithFlag)
	{
		if (HasFlag(inv.m_flags, DoubleClickGather) == false)
			continue;

		for (InventorySlot* s : *inv.m_slots)
		{
			if (s == nullptr || s->IsEmpty() || s->GetStack().GetID() != id)
				continue;

			int space = max - m_cursorStack.GetAmount();

			if (space <= 0)
				return;

			int move = std::min(space, s->GetStack().GetAmount());
			m_cursorStack.SetAmount(m_cursorStack.GetAmount() + move);
			s->GetStack().SetAmount(s->GetStack().GetAmount() - move);

			if (s->GetStack().GetAmount() <= 0)
				s->Clear();
			else
				s->RefreshVisual();
		}
	}
}

void InventoryManager::HandleDropOutside()
{
	m_cursorStack.Clear();
	m_state = State::Idle;
	m_pressedSlot = nullptr;
	m_dragSlots.clear();
	m_dragOriginalAmount.clear();
}

void InventoryManager::PickupStack(InventorySlot* slot)
{
	if (CanInteract(slot, PickItem) == false || slot->IsEmpty())
		return;

	m_cursorStack = slot->GetStack();
	slot->Clear();
}

void InventoryManager::PlaceFullStack(InventorySlot* slot)
{
	if (CanInteract(slot, PlaceItem) == false || slot->IsEmpty() == false || HasCursorItem() == false)
		return;

	slot->SetStack(m_cursorStack);
	m_cursorStack.Clear();
}

void InventoryManager::SwapStacks(InventorySlot* slot)
{
	if (CanInteract(slot, SwapItem) == false || HasCursorItem() == false)
		return;

	ItemStack tmp = slot->GetStack();
	slot->SetStack(m_cursorStack);
	m_cursorStack = tmp;
}

void InventoryManager::MergeStacks(InventorySlot* slot)
{
	if (CanInteract(slot, MergeStack) == false || slot->IsEmpty() || HasCursorItem() == false)
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
	slot->GetStack().SetAmount(slot->GetStack().GetAmount() + toMove);
	m_cursorStack.SetAmount(m_cursorStack.GetAmount() - toMove);

	if (m_cursorStack.GetAmount() <= 0)
		m_cursorStack.Clear();

	slot->RefreshVisual();
}

void InventoryManager::SplitStackToCursor(InventorySlot* slot)
{
	if (CanInteract(slot, SplitStack) == false || slot->IsEmpty())
		return;

	int total = slot->GetStack().GetAmount();
	int half = (total + 1) / 2;

	m_cursorStack = slot->GetStack();
	m_cursorStack.SetAmount(half);

	slot->GetStack().SetAmount(total - half);

	if (slot->GetStack().GetAmount() <= 0)
		slot->Clear();
	else
		slot->RefreshVisual();
}

void InventoryManager::PlaceSingleItem(InventorySlot* slot)
{
	if (CanInteract(slot, PlaceItem) == false || HasCursorItem() == false)
		return;

	int max = GetMaxStack(m_cursorStack.GetID());

	if (max <= 0)
		return;

	if (slot->IsEmpty())
	{
		ItemStack single;
		single.SetID(m_cursorStack.GetID());
		single.SetAmount(1);
		slot->SetStack(single);
	}
	else
	{
		if (slot->GetStack().GetID() != m_cursorStack.GetID())
			return;

		if (slot->GetStack().GetAmount() >= max)
			return;

		slot->GetStack().SetAmount(slot->GetStack().GetAmount() + 1);
		slot->RefreshVisual();
	}

	m_cursorStack.SetAmount(m_cursorStack.GetAmount() - 1);

	if (m_cursorStack.GetAmount() <= 0)
		m_cursorStack.Clear();
}

void InventoryManager::RedistributeDrag()
{
	if (m_dragSlots.empty())
		return;

	int n = static_cast<int>(m_dragSlots.size());
	int max = GetMaxStack(m_dragItemID);

	if (max <= 0)
		return;

	std::vector<int> capacity(n);
	int totalCapacity = 0;

	for (int i = 0; i < n; i++)
	{
		capacity[i] = max - m_dragOriginalAmount[i];
		totalCapacity += capacity[i];
	}

	int toDistribute = std::min(m_dragTotalAtStart, totalCapacity);

	int perSlot = toDistribute / n;
	int extra = toDistribute - perSlot * n;

	for (int i = 0; i < n; i++)
	{
		ItemStack preview;
		preview.SetID(m_dragItemID);
		preview.SetAmount(perSlot);
		m_dragSlots[i]->SetStack(preview);
	}

	m_cursorStack.SetID(m_dragItemID);
	m_cursorStack.SetAmount(extra);
}

void InventoryManager::CommitDrag()
{
	int n = static_cast<int>(m_dragSlots.size());

	if (n == 0)
		return;

	int max = GetMaxStack(m_dragItemID);
	if (max <= 0)
	{
		m_cursorStack.Clear();
		return;
	}

	int totalCapacity = 0;

	for (int i = 0; i < n; i++)
		totalCapacity += max - m_dragOriginalAmount[i];

	int toDistribute = std::min(m_dragTotalAtStart, totalCapacity);

	int perSlot = toDistribute / n;
	int extra = toDistribute - perSlot * n;

	if (extra <= 0)
		m_cursorStack.Clear();
	else
	{
		m_cursorStack.SetID(m_dragItemID);
		m_cursorStack.SetAmount(extra);
	}
}

void InventoryManager::CancelDrag()
{
	for (InventorySlot* s : m_dragSlots)
		s->Clear();

	m_cursorStack.SetAmount(m_dragTotalAtStart);
	m_dragSlots.clear();
	m_dragOriginalAmount.clear();
	m_state = State::Holding;
}

bool InventoryManager::HasFlag(uint8_t flags, InventoryInterractionBit flag)
{
	return (flags & flag) != 0;
}

InventoryManager::InventoryWithFlag* InventoryManager::FindInventory(InventorySlot* slot)
{
	for (InventoryWithFlag& inv : m_iventoriesWithFlag)
	{
		for (InventorySlot* s : *inv.m_slots)
		{
			if (s == slot)
				return &inv;
		}
	}

	return nullptr;
}

bool InventoryManager::CanInteract(InventorySlot* slot, InventoryInterractionBit flag)
{
	if (slot == nullptr)
		return false;

	InventoryWithFlag* inv = FindInventory(slot);

	if (inv == nullptr)
		return false;

	return HasFlag(inv->m_flags, flag);
}

int InventoryManager::GetMaxStack(const std::string& id)
{
	ItemData* data = ItemDataBase::Get()->GetItem(id);

	if (data == nullptr)
		return -1;

	return data->GetMaxStack();
}