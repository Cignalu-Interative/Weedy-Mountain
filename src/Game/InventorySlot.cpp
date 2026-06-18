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
        if (m_isVisible)
        {
            SetSprite("item_texture", data->GetPath());
            SetText(std::to_string(m_stack.GetAmount()));
            SetTextOffset({ 18.0f, 22.0f });
        }
        else
        {
            SetSprite("item_texture", "");
            SetText("");
            SetTextOffset({ 18.0f, 22.0f });
        }
    }
}

void InventorySlot::OnUpdate()
{
    Button::OnUpdate();

    if (IsMouseIn()) //FIX 
    {
        SetSprite("InventorySlotHover");
    }
    else
    {
        SetSprite("InventorySlot");
    }
}