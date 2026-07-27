#include "pch.h"
#include "ItemData.h"

ItemData::ItemData(std::string ID, std::string path, std::string name,
    std::string description, int buyPrice, int sellPrice, int maxStack,
    std::string useActionID, ItemType itemType,
    int weaponDamage,
    ToolRarity toolRarity, ToolType toolType,
    int consumableEnergyRestored, int consumableFatigueReduced,
    float cookTime, std::string cookResultItemID)
    : m_ID(std::move(ID)),
    m_path(std::move(path)),
    m_name(std::move(name)),
    m_description(std::move(description)),
    m_buyPrice(buyPrice),
    m_sellPrice(sellPrice),
    m_maxStack(maxStack),
    m_useActionID(std::move(useActionID)),
    m_itemType(itemType),
    m_weaponDamage(weaponDamage),
    m_toolRarity(toolRarity),
    m_toolType(toolType),
    m_consumableEnergyRestored(consumableEnergyRestored),
    m_consumableFatigueReduced(consumableFatigueReduced),
    m_cookTime(cookTime),
    m_cookResultItemID(std::move(cookResultItemID))
{}

const std::string& ItemData::GetID() const
{
	return m_ID;
}

const std::string& ItemData::GetPath() const
{
	return m_path;
}

const std::string& ItemData::GetName() const
{
	return m_name;
}

const std::string& ItemData::GetDescription() const
{
	return m_description;
}

int ItemData::GetBuyPrice() const
{
	return m_buyPrice;
}

int ItemData::GetSellPrice() const
{
	return m_sellPrice;
}

int ItemData::GetMaxStack() const
{
	return m_maxStack;
}

const std::string& ItemData::GetUseActionID() const
{
	return m_useActionID; 
}

ItemType ItemData::GetItemType() const { return m_itemType; }

int ItemData::GetWeaponDamage() const { return m_weaponDamage; }

ToolRarity ItemData::GetToolRarity() const { return m_toolRarity; }
ToolType ItemData::GetToolType() const { return m_toolType; }

int ItemData::GetConsumableEnergyRestored() const { return m_consumableEnergyRestored; }
int ItemData::GetConsumableFatigueReduced() const { return m_consumableFatigueReduced; }

bool ItemData::IsCookable() const { return m_cookTime > 0.f; }

float ItemData::GetCookTime() const { return m_cookTime; }
const std::string& ItemData::GetCookResultItemID() const { return m_cookResultItemID; }

void ItemData::SetCookResultItemID(std::string resolvedID)
{
    m_cookResultItemID = std::move(resolvedID);
}
