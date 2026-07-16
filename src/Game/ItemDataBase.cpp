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

ItemDataBase* ItemDataBase::Get()
{
	static ItemDataBase instance;
	return &instance;
}

bool ItemDataBase::LoadFromJSON(const std::string& filePath, std::function<bool(const std::string&)> validateActionID)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return false;

    json root;
    try { root = json::parse(file); }
    catch (...) { return false; }

    if (!root.is_array())
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

        m_items.try_emplace(id, ItemData{ id, stem, name, description, buyPrice, sellPrice, maxStack });
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
