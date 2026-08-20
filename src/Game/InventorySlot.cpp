#include "pch.h"
#include "InventorySlot.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "Entity.h"
#include "spritecontainer.h"

InventorySlot::InventorySlot()
	: m_stack("ITEM_NOTHING", 0) 
{
    SetDrawLayer(5999);
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
        SetDrawLayer(5999);
        SetText(" ");
        return;
    }

    ItemData* data = ItemDataBase::Get()->GetItem(m_stack.GetID());

    if (data) 
    {
        if (m_isVisible)
        {
            /*SetSprite("item_texture", data->GetPath());
            GetSprite("item_texture")->ResizeStretched(GetDimension());*/

            RemoveSprite("item_texture");
            AddSprite("item_texture", data->GetPath(), { 54,54 }, 1, { 0,0 }, true);
            SetDrawLayer(5999);
            GetSprite("item_texture")->Resize({ 54, 54 });
            SyncSprites();
            SetText(std::to_string(m_stack.GetAmount()));
            SetTextOffset({ 18.0f, 22.0f });
        }
        else
        {
            SetSprite("item_texture", "");
            SetText("");
            SetTextOffset({ 18.0f, 22.0f });
            SetDrawLayer(5999);
        }
    }
}