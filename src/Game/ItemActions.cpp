#include "pch.h"
#include "ItemActions.h"
#include "ItemActionRegistry.h"
#include "ItemDataBase.h"
#include "PlayerInventory.h"
#include "Player.h"
#include "FarmManager.h"
#include "FarmTile.h"
#include "CropDefinition.h"
#include "GameData.h"

#include <iostream>
#include <unordered_map>
#include <functional>

static const std::unordered_map<std::string, std::function<CropDefinition()>> s_seedToCrop =
{
    { "ITEM_WHEAT_SEED", []() { return CropCatalog::Wheat(); } },
    { "ITEM_CARROT_SEED", []() { return CropCatalog::Carrot(); } },
};

static bool TryGetCropForSeed(const std::string& itemID, CropDefinition& outCrop)
{
    auto it = s_seedToCrop.find(itemID);
    if (it == s_seedToCrop.end())
        return false;

    outCrop = it->second();
    return true;
}

void RegisterAllItemActions()
{
    ItemActionRegistry* registry = ItemActionRegistry::Get();

    registry->Register("PLANT_SEED", [](Player& player, PlayerInventory& inv, const std::string& itemID, int slotIndex)
        {
            CropDefinition crop;
            if (TryGetCropForSeed(itemID, crop) == false)
            {
                std::cerr << "[ItemActions] PLANT_SEED : itemID \"" << itemID << "\" non reconnu dans s_seedToCrop (voir ItemActions.cpp)" << std::endl;
                return;
            }

            Collider* actionBox = player.GetActionBoxCollider();
            if (actionBox == nullptr)
                return;

            FarmTile* target = FarmManager::Get()->GetTileInActionBox(actionBox);
            if (target == nullptr)
                return;

            FarmTileState state = target->GetState();
            if (state != FarmTileState::TILLED && state != FarmTileState::WATERED)
                return;  

            int currentDay = GameData::Get().GetDay();
            target->Plant(crop, currentDay);

            inv.RemoveItem(itemID, 1, false);
        });

    registry->Register("WATER_CROP", [](Player& player, PlayerInventory& inv, const std::string& itemID, int slotIndex)
        {
            Collider* actionBox = player.GetActionBoxCollider();
            if (actionBox == nullptr)
                return;

            FarmTile* target = FarmManager::Get()->GetTileInActionBox(actionBox);
            if (target == nullptr)
                return;

            FarmTileState state = target->GetState();

            if (state != FarmTileState::TILLED && state != FarmTileState::PLANTED && state != FarmTileState::GROWING)
                return;

            int currentDay = GameData::Get().GetDay();
            target->Water(currentDay);
        });

    registry->Register("USE_FISHING_ROD", [](Player& player, PlayerInventory& inv, const std::string& itemID, int slotIndex)
        {
            player.StartFishing();
        });
}