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
	RecipeBlock* m_lastRecipeBlockSelected = nullptr;
	RecipeBlock* m_actualRecipeBlockSelected = nullptr;
	void ChangeSelectedRecipe();
	void OnUpdate() override;
public :

	void Initialize(std::vector<RecipeData*> p_recipeVector, int p_startIndex);

	//void Update(); //Update les recipes block dans le menu grace au slider
	void SetIsVisible(bool p_isVisible);
	std::vector<RecipeBlock*> GetRecipeBlock();

	int GetEndRecipeIndex() { return m_endRecipeIndex; }

	void ResetSelectedRecipe();

	RecipeBlock* GetSelectedRecipe() { return m_actualRecipeBlockSelected; }
};

