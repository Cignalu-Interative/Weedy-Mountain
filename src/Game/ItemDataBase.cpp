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
            "Item_Wood",
            "Wood",
            "Wood plank",
            1,
            3,
            64
        }
    );

    m_items.try_emplace
    (
        ITEM_FIBER,

        ItemData
        {
            ITEM_FIBER,
            "Item_Fiber",
            "Fiber",
            "Fiber",
            1,
            3,
            64
        }

    );

    m_items.try_emplace
    (
        ITEM_ROCK,

        ItemData
        {
            ITEM_ROCK,
            "Item_Rock",
            "Rock",
            "Rock",
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
