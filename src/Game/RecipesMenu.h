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
	
	int m_endRecipeIndex;
public :

	void Initialize(std::vector<RecipeData*> p_recipeVector, int p_startIndex);

	//void Update(); //Update les recipes block dans le menu grace au slider
	void SetIsVisible(bool p_isVisible);
	std::vector<RecipeBlock*> GetRecipeBlock();

	int GetEndRecipeIndex() { return m_endRecipeIndex; }
};

