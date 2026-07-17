#include "pch.h"
#include "RecipeData.h"

RecipeData::RecipeData(std::string recipeID, std::string resultItemID, int resultQuantity, std::vector<RecipeIngredientData> ingredients) : m_recipeID(std::move(recipeID)), m_resultItemID(std::move(resultItemID)), m_resultQuantity(resultQuantity), m_ingredients(std::move(ingredients)) {}

const std::string& RecipeData::GetRecipeID() const { return m_recipeID; }
const std::string& RecipeData::GetResultItemID() const { return m_resultItemID; }
int RecipeData::GetResultQuantity() const { return m_resultQuantity; }

const std::vector<RecipeIngredientData>& RecipeData::GetIngredients() const { return m_ingredients; }