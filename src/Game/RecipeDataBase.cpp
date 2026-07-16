#include "pch.h"
#include "RecipeDataBase.h"
#include "ItemDataBase.h"

#include "2DpendentLib/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

RecipeDataBase* RecipeDataBase::Get()
{
    static RecipeDataBase instance;
    return &instance;
}

bool RecipeDataBase::LoadFromJSON(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (file.is_open() == false)
        return false;

    json root;
    try { root = json::parse(file); }
    catch (...) { return false; }

    if (root.is_array() == false)
        return false;

    m_recipes.clear();

    for (const auto& itemObj : root)
    {
        std::string itemName = itemObj.value("name", "");
        int itemInternalID = itemObj.value("id", -1);

        if (itemName.empty() || itemInternalID < 0)
            continue;

        const std::string& resultItemID = ItemDataBase::Get()->ResolveInternalID(itemInternalID);

        if (resultItemID.empty())
        {
            std::cerr << "[RecipeDataBase] Warning: impossible de resoudre l'item \"" << itemName << "\" (ItemDataBase::LoadFromJSON a-t-il ete appele avant ?)" << std::endl;
            continue;
        }

        if (itemObj.contains("recipes") == false || itemObj["recipes"].is_array() == false)
            continue;

        for (const auto& recipeObj : itemObj["recipes"])
        {
            std::string recipeID = recipeObj.value("recipeID", "");
            int resultQuantity = recipeObj.value("resultQuantity", 1);

            if (recipeID.empty())
                continue;

            std::vector<RecipeIngredientData> ingredients;

            if (recipeObj.contains("ingredients") && recipeObj["ingredients"].is_array())
            {
                for (const auto& ingObj : recipeObj["ingredients"])
                {
                    int ingInternalID = ingObj.value("itemID", -1);
                    int ingQuantity = ingObj.value("quantity", 1);

                    const std::string& ingStringID = ItemDataBase::Get()->ResolveInternalID(ingInternalID);

                    if (ingStringID.empty())
                    {
                        std::cerr << "[RecipeDataBase] Warning: recette \"" << recipeID << "\" reference un ingredient inconnu (internal id " << ingInternalID << ")" << std::endl;
                        continue;
                    }

                    RecipeIngredientData ing;
                    ing.itemID = ingStringID;
                    ing.quantity = ingQuantity;
                    ingredients.push_back(ing);
                }
            }

            if (m_recipes.find(recipeID) != m_recipes.end())
            {
                std::cerr << "[RecipeDataBase] Warning: recipeID \"" << recipeID << "\" en doublon, la recette precedente est ecrasee." << std::endl;
            }

            m_recipes[recipeID] = RecipeData{ recipeID, resultItemID, resultQuantity, std::move(ingredients) };
        }
    }

    return true;
}

RecipeData* RecipeDataBase::GetRecipe(const std::string& recipeID)
{
    auto it = m_recipes.find(recipeID);

    if (it != m_recipes.end())
        return &it->second;

    return nullptr;
}

const std::unordered_map<std::string, RecipeData>& RecipeDataBase::GetAll() const
{
    return m_recipes;
}

std::vector<RecipeData*> RecipeDataBase::GetRecipesForItem(const std::string& itemID)
{
    std::vector<RecipeData*> result;

    for (auto& pair : m_recipes)
    {
        if (pair.second.GetResultItemID() == itemID)
            result.push_back(&pair.second);
    }

    return result;
}