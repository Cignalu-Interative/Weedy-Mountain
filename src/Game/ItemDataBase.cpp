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
        ITEM_EGG,

        ItemData
        {
            ITEM_EGG,
            "Egg",
            "OEUF",
            "Oeuf de poule... Surement...",
            1,
            3,
            64
        }
    );

    m_items.try_emplace
    (
        ITEM_POTATOE,

        ItemData
        {
            ITEM_POTATOE,
            "Potatoe",
            "PATATE",
            "Elle te regarde avec ses yeux...",
            1,
            3,
            64
        }
    );

    m_items.try_emplace
    (
        ITEM_SUN_FLOWER,

        ItemData
        {
            ITEM_SUN_FLOWER,
            "SunFlower",
            "Tournesol",
            "Quasiment necessaire contre les zombies",
            1,
            3,
            64
        }
    );

    m_items.try_emplace
    (
        ITEM_FISHING_ROD,

        ItemData
        {
            ITEM_FISHING_ROD,
            "FishingRod",
            "Canne A Peche",
            "T'as vu de l'eau quelque part ???",
            5,
            30,
            1
        }
    );

    m_items.try_emplace
    (
        ITEM_HELMET,

        ItemData
        {
            ITEM_HELMET,
            "Helmet",
            "Casque",
            "J'ai pas d'idee de connerie a mettre ici :/",
            5,
            30,
            1
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
