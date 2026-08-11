#include "pch.h"
#include "RecipesMenu.h"
#include "ItemDataBase.h"
#include "RecipeDataBase.h"
#include "RecipeBlock.h"	
#include "RecipeData.h"
#include "SpriteContainer.h"

void RecipesMenu::OnInitialize()
{
	RemoveCollider("default");
}

void RecipesMenu::PlaceRecipeBlock()
{
	float previousPosY = GetPosition({ 0.5,0 }).GetY();
	Vector2f bottomLeftPos = GetPosition({ 0,1 });
	float posX = GetPosition().GetX();
	for(int i = 0; i< m_recipes.size(); i++)
	{
		if(i == 0)
			previousPosY += 100;
		else
			previousPosY += 150;

		if (previousPosY > bottomLeftPos.GetY())
		{
			return;
		}
		m_recipes[i]->SetPosition({ posX, previousPosY });
	}
}

int RecipesMenu::PlaceRecipeBlock(std::vector<RecipeBlock*> p_recipeVector, int p_startIndex)
{
	std::vector<RecipeBlock*> newRecipes;
	Vector2f bottomLeftPos = GetPosition({ 0,1 });
	float previousPosY = GetPosition({ 0.5,0 }).GetY();
	int nextIndex = -1;

	for(int  i = p_startIndex; i < p_recipeVector.size(); i++)
	{
		if (i == p_startIndex)
			previousPosY += 100;
		else
			previousPosY += 150;

		if (previousPosY > bottomLeftPos.GetY())
		{
			nextIndex = i;
			break;
		}
		p_recipeVector[i]->SetPosition({ GetPosition().GetX(), previousPosY });
		newRecipes.push_back(p_recipeVector[i]);
	}

	m_recipes.clear();
	m_recipes = newRecipes;

	return nextIndex;
}

void RecipesMenu::SetIsVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);

	for (RecipeBlock* block : m_recipes)
	{
		block->SetIsVisible(p_isVisible);
	}
}

std::vector<RecipeBlock*> RecipesMenu::GetRecipeBlock()
{
	return m_recipes;
}
