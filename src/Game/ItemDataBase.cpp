#include "pch.h"
#include "ItemDataBase.h"

//Initialiser les items dans le constructeur

ItemDataBase* ItemDataBase::Get()
{
	static ItemDataBase instance;
	return &instance;
}


ItemDataBase::ItemDataBase()
{
    m_items.try_emplace
    (
        ITEM_WOOD,

        ItemData
        {
            ITEM_WOOD,
            "Wood",
            "Wood",
            "Wood plank",
            1,
            3,
            64
        }
    );
}

ItemData* ItemDataBase::GetItem(ItemID ID)
{
    auto it = m_items.find(ID);

    if (it != m_items.end())
        return &it->second;

    return nullptr;
}
