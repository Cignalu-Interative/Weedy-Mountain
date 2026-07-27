#pragma once
#include <string>
#include <variant>
#include "ItemType.h"

class ItemData
{
private:
	
	std::string m_ID;

	std::string m_path;

	std::string m_name;
	std::string m_description;

	int m_buyPrice = 0;
	int m_sellPrice = 0;

	int m_maxStack = 64;

	std::string m_useActionID;

	ItemType m_itemType = ItemType::Default;

	// Weapon
	int m_weaponDamage = 0;

	// Tool
	ToolRarity m_toolRarity = ToolRarity::Wooden;
	ToolType   m_toolType = ToolType::Axe;

	// Consumable
	int m_consumableEnergyRestored = 0;
	int m_consumableFatigueReduced = 0;

	float m_cookTime = 0.f;   
	std::string m_cookResultItemID;

public:

	ItemData(std::string ID, std::string path, std::string name,
		std::string description, int buyPrice, int sellPrice, int maxStack,
		std::string useActionID, ItemType itemType,
		int weaponDamage,
		ToolRarity toolRarity, ToolType toolType,
		int consumableEnergyRestored, int consumableFatigueReduced,
		float cookTime, std::string cookResultItemID);

	const std::string& GetID() const;
	const std::string& GetPath() const;
	const std::string& GetName() const;
	const std::string& GetDescription() const;
	int GetBuyPrice() const;
	int GetSellPrice() const;
	int GetMaxStack() const;
	const std::string& GetUseActionID() const;

	ItemType GetItemType() const;

	// Weapon
	int GetWeaponDamage() const;

	// Tool
	ToolRarity GetToolRarity() const;
	ToolType GetToolType() const;

	// Consumable
	int GetConsumableEnergyRestored() const;
	int GetConsumableFatigueReduced() const;

	bool IsCookable() const;

	float GetCookTime() const;
	const std::string& GetCookResultItemID() const;

	void SetCookResultItemID(std::string resolvedID);
};