#pragma once
#include "Entity.h"
#include "Slider.h"
#include <vector>

class RecipeData;
class RecipeBlock;
class RecipesMenu : public Entity
{
private :

	std::vector<RecipeBlock*> m_recipes;

	void OnInitialize() override;
public :

	//void Update(); //Update les recipes block dans le menu grace au slider
	void SetIsVisible(bool p_isVisible);
	std::vector<RecipeBlock*> GetRecipeBlock();

	void SetRecipeVector(std::vector<RecipeBlock*> p_recipeVector) { m_recipes.clear(); m_recipes = p_recipeVector; }

	void PlaceRecipeBlock();

	int PlaceRecipeBlock(std::vector<RecipeBlock*> p_recipeVector, int p_startIndex = 0);
};

