#pragma once
#include <string>
#include <vector>

struct RecipeIngredientData
{
    std::string itemID;
    int quantity = 1;
};

class RecipeData
{
private:

    std::string m_recipeID;                       
    std::string m_resultItemID;                     
    int m_resultQuantity = 1;

    std::vector<RecipeIngredientData> m_ingredients;

public:

    RecipeData() = default;

    RecipeData(std::string recipeID, std::string resultItemID, int resultQuantity, std::vector<RecipeIngredientData> ingredients);

    const std::string& GetRecipeID() const;
    const std::string& GetResultItemID() const;
    int GetResultQuantity() const;
    const std::vector<RecipeIngredientData>& GetIngredients() const;
};