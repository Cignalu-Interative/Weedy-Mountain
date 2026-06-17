#include "pch.h"
#include "InventorySlot.h"
#include "ItemData.h"
#include "ItemDataBase.h"

InventorySlot::InventorySlot()
	: m_stack(ITEM_NOTHING, 0) {}

bool InventorySlot::IsEmpty()
{
	return m_stack.IsEmpty();
}

void InventorySlot::Clear()
{
	m_stack.Clear();
    RefreshVisual();
}

ItemStack& InventorySlot::GetStack()
{
	return m_stack;
}

void InventorySlot::SetStack(const ItemStack& stack)
{
	m_stack = stack;
    RefreshVisual();
}

void InventorySlot::RefreshVisual()
{
    if (m_stack.IsEmpty())
    {
        SetSprite("InventorySlot");
        SetText(" ");
        return;
    }

    ItemData* data = ItemDataBase::Get()->GetItem(m_stack.GetID());

    if (data) 
    {
        SetSprite(data->GetPath());
        SetText(std::to_string(m_stack.GetAmount()));
        SetTextOffset({ 18.0f, 22.0f });
    }
}