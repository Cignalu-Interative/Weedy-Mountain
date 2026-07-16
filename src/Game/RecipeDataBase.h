#pragma once

#include "RecipeData.h"
#include <unordered_map>
#include <string>

class RecipeDataBase
{
private:
    std::unordered_map<std::string, RecipeData> m_recipes;

    RecipeDataBase() = default;

public:

    static RecipeDataBase* Get();

    bool LoadFromJSON(const std::string& filePath);

    RecipeData* GetRecipe(const std::string& recipeID);

    const std::unordered_map<std::string, RecipeData>& GetAll() const;

    std::vector<RecipeData*> GetRecipesForItem(const std::string& itemID);
};