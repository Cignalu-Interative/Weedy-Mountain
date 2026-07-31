#include "pch.h"
#include "PlayerCollection.h"
#include "ItemDataBase.h"
#include <algorithm>

void PlayerCollection::UnlockItem(const std::string& itemID)
{
    if (IsUnlocked(itemID))
        return;

    m_unlockedItems.push_back(itemID);
}

bool PlayerCollection::IsUnlocked(const std::string& itemID) const
{
    return std::find(m_unlockedItems.begin(), m_unlockedItems.end(), itemID) != m_unlockedItems.end();
}

const std::vector<std::string>& PlayerCollection::GetUnlockedItems() const
{
    return m_unlockedItems;
}

ItemDisplayInfo PlayerCollection::GetDisplayInfo(const std::string& itemID) const
{
    ItemDisplayInfo info;
    info.m_unlocked = IsUnlocked(itemID);

    ItemData* item = ItemDataBase::Get()->GetItem(itemID);
    if (!item)
    {
        info.m_name = "";
        info.m_iconPath = "lockedItem";
        info.m_texturePath = "LockedRecipe";
        return info;
    }

    if (info.m_unlocked)
    {
        info.m_name = item->GetName();
        info.m_description = item->GetDescription();
        info.m_iconPath = item->GetPath();
        info.m_texturePath = "InventoryWeedyMountain";
    }
    else
    {
        info.m_name = "";
        info.m_description = "Not Discovered";
        info.m_iconPath = "lockedItem";
        info.m_texturePath = "LockedRecipe";
    }
    return info;
}