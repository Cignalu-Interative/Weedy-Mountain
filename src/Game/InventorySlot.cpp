#include "pch.h"
#include "InventorySlot.h"

InventorySlot::InventorySlot()
	: m_stack("ITEM_NOTHING", 0) {
}

bool InventorySlot::IsEmpty()
{
	return m_stack.IsEmpty();
}

void InventorySlot::Clear()
{
	m_stack.Clear();
}

ItemStack& InventorySlot::GetStack()
{
	return m_stack;
}

void InventorySlot::SetStack(const ItemStack& stack)
{
	m_stack = stack;
}