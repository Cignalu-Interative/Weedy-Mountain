#include "pch.h"
#include "ItemDataBase.h"

#include "2DpendentLib/json.hpp"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

static std::string ExtractStemFromPath(const std::string& raw)
{
    return std::filesystem::path(raw).stem().string();
}

static std::string MakeItemID(const std::string& name)
{
    std::string id = "ITEM_";
    for (char c : name)
    {
        if (c == ' ' || c == '-')
            id += '_';
        else
            id += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return id;
}

static ItemType ParseItemType(const std::string& s)
{
    if (s == "Weapon")
        return ItemType::Weapon;

    else if (s == "Tool")       
        return ItemType::Tool;

    else if (s == "Consumable") 
        return ItemType::Consumable;

    return ItemType::Default;
}

static ToolRarity ParseToolRarity(const std::string& s)
{
    if (s == "Wooden")
        return ToolRarity::Wooden;

    else if (s == "Stone")   
        return ToolRarity::Stone;

    else if (s == "Iron")   
        return ToolRarity::Iron;

    else if (s == "Gold")    
        return ToolRarity::Gold;

    else if (s == "Diamond") 
        return ToolRarity::Diamond;

    return ToolRarity::Wooden;
}

static ToolType ParseToolType(const std::string& s)
{
    if (s == "Hoe")         return ToolType::Hoe;
    if (s == "Pickaxe")     return ToolType::Pickaxe;
    if (s == "Spade")       return ToolType::Spade;
    if (s == "WateringCan") return ToolType::WateringCan;
    return ToolType::Axe;
}

ItemDataBase* ItemDataBase::Get()
{
	static ItemDataBase instance;
	return &instance;
}

bool ItemDataBase::LoadFromJSON(const std::string& filePath, std::function<bool(const std::string&)> validateActionID)
{
    std::ifstream file(filePath);
    if (file.is_open() == false)
        return false;

    json root;
    try { root = json::parse(file); }
    catch (...) { return false; }

    if (root.is_array() == false)
        return false;

    m_items.clear();

    for (const auto& obj : root)
    {
        std::string name = obj.value("name", "");
        std::string rawPath = obj.value("imagePath", "");
        std::string description = obj.value("description", "");
        int buyPrice = obj.value("buyPrice", 0);
        int sellPrice = obj.value("sellPrice", 0);
        int maxStack = obj.value("maxStack", 64);
        std::string useActionID = obj.value("useActionID", "");
        int internalID = obj.value("id", -1);

        ItemType itemType = ParseItemType(obj.value("itemType", "Default"));
        int weaponDamage = obj.value("weaponDamage", 0);
        ToolRarity toolRarity = ParseToolRarity(obj.value("toolRarity", "Wooden"));
        ToolType toolType = ParseToolType(obj.value("toolType", "Axe"));
        int consumableEnergyRestored = obj.value("consumableEnergyRestored", 0);
        int consumableFatigueReduced = obj.value("consumableFatigueReduced", 0);

        float cookTime = obj.value("cookTime", 0.f);

        if (name.empty())
            continue;

        std::string id = MakeItemID(name);
        std::string stem = ExtractStemFromPath(rawPath);

        if (useActionID.empty() == false && validateActionID != nullptr)
        {
            if (validateActionID(useActionID) == false)
            {
                std::cerr << "[ItemDataBase] Warning: item \"" << name << "\" reference une action inconnue \"" << useActionID << "\"" << std::endl;
            }
        }

        m_items.try_emplace(id, ItemData{ id, stem, name, description, buyPrice, sellPrice, maxStack, useActionID, itemType, weaponDamage, toolRarity, toolType, consumableEnergyRestored, consumableFatigueReduced, cookTime, std::string{} });

        if (internalID >= 0)
            m_internalIdToStringId[internalID] = id;

    }

    for (const auto& obj : root)
    {
        std::string name = obj.value("name", "");

        if (name.empty()) 
            continue;

        float cookTime = obj.value("cookTime", 0.f);

        if (cookTime <= 0.f)
            continue;   

        int cookResultInternalID = obj.value("cookResultItemID", -1);

        if (cookResultInternalID < 0) 
            continue;

        const std::string& resolvedResultID = ResolveInternalID(cookResultInternalID);
        if (resolvedResultID.empty())
        {
            std::cerr << "[ItemDataBase] Warning: item \"" << name << "\" a un cookResultItemID introuvable (internal id " << cookResultInternalID << ")" << std::endl;
            continue;
        }

        std::string id = MakeItemID(name);
        auto it = m_items.find(id);

        if (it != m_items.end())
            it->second.SetCookResultItemID(resolvedResultID);
    }

    return true;
}

ItemData* ItemDataBase::GetItem(const std::string& ID)
{
    auto it = m_items.find(ID);

    if (it != m_items.end())
        return &it->second;

    return nullptr;
}

const std::unordered_map<std::string, ItemData>& ItemDataBase::GetAll() const
{
    return m_items;
}

const std::string& ItemDataBase::ResolveInternalID(int internalID) const
{
    static const std::string empty;

    auto it = m_internalIdToStringId.find(internalID);

    if (it != m_internalIdToStringId.end())
        return it->second;

    return empty;
}
