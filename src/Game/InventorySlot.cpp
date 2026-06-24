#include "pch.h"
#include "InventorySlot.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "2DPendentLib/InputManager.h"

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
        SetSprite("item_texture", "");
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

    InputManager* im = InputManager::Get();
    bool mouseIn = IsMouseIn();

    if (mouseIn && !m_wasMouseIn)
    {
        if (im->IsMouseHeld(BUTTON_LEFT) && m_onLeftHeldEnter)
            m_onLeftHeldEnter();

        if (im->IsMouseHeld(BUTTON_RIGHT) && m_onRightHeldEnter)
            m_onRightHeldEnter();
    }

    if (im->IsMouseUp(BUTTON_LEFT) && m_onLeftRelease)
        m_onLeftRelease();

    if (im->IsMouseUp(BUTTON_RIGHT) && m_onRightRelease)
        m_onRightRelease();

    m_wasMouseIn = mouseIn;
}