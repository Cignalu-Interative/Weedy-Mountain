#pragma once
#include "Slider.h"
#include <vector>

class RecipeBlock;
class RecipesMenu
{
private :

	std::vector<RecipeBlock*> m_recipes;

	
public :

	void Initialize();

	//void Update(); //Update les recipes block dans le menu grace au slider

	int GetLastIndexRecipeAdd();
};

