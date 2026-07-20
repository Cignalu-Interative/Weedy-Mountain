#include "pch.h"
#include "RecipesMenu.h"
#include "ItemDataBase.h"
#include "RecipeDataBase.h"
#include "RecipeBlock.h"	
#include "RecipeData.h"

void RecipesMenu::Initialize(std::vector<RecipeData*> p_recipeVector, int p_startIndex)
{
	RemoveCollider("default");
	ItemDataBase* item = ItemDataBase::Get();
	RecipeDataBase* recipe = RecipeDataBase::Get();

	Vector2f recipesMenuPos = GetPosition();

	Vector2f bottomLeftPos = GetPosition({0,1});
	float previousPosY =GetPosition({0.5,0}).GetY();
	int number = 1;

	for (int i = p_startIndex ; i < p_recipeVector.size(); i++)
	{
		if (i == 0)
			previousPosY += 100;
		else
			previousPosY += 150;

		if (previousPosY > bottomLeftPos.GetY())
		{
			return;
		}

		RecipeBlock* recipeBlock = GetScene()->CreateEntity<RecipeBlock>({GetPosition().GetX(), previousPosY}, {GetDimension().GetX(),150}, "InventoryWeedyMountain", Shape::Rectangle, true);
		recipeBlock->Initialize(p_recipeVector[i]->GetRecipeID());
		previousPosY = recipeBlock->GetPosition().GetY();
		//recipeBlock->SetPosition(GetPosition({}), {});

		Vector2f bottomPos = recipeBlock->GetPosition({ 0,1 });

		m_recipes.push_back(recipeBlock);
		m_endRecipeIndex = i;
		number++;
	}
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
