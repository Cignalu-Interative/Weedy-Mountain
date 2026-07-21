#include "pch.h"
#include "RecipesMenu.h"
#include "ItemDataBase.h"
#include "RecipeDataBase.h"
#include "RecipeBlock.h"	
#include "RecipeData.h"

void RecipesMenu::ChangeSelectedRecipe()
{
	for (RecipeBlock* block : m_recipes)
	{

		if (block != m_actualRecipeBlockSelected)
			block->GetSprite()->ChangeTexture("InventoryWeedyMountain");
	}
}

void RecipesMenu::ResetSelectedRecipe()
{
	for (RecipeBlock* block : m_recipes)
	{
		block->SetIsSelected(false);
		block->GetSprite()->ChangeTexture("InventoryWeedyMountain");
	}
}

void RecipesMenu::OnUpdate()
{
	ChangeSelectedRecipe();
}

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
		if (i == p_startIndex)
			previousPosY += 100;
		else
			previousPosY += 150;

		if (previousPosY > bottomLeftPos.GetY())
		{
			return;
		}

		RecipeBlock* recipeBlock = GetScene()->CreateEntity<RecipeBlock>({GetPosition().GetX(), previousPosY}, {GetDimension().GetX(),150}, "InventoryWeedyMountain", Shape::Rectangle, true);
		recipeBlock->Initialize(p_recipeVector[i]->GetRecipeID());
		recipeBlock->SetOnLeftClick([this, recipeBlock] {
			m_lastRecipeBlockSelected = m_actualRecipeBlockSelected;
			m_actualRecipeBlockSelected = recipeBlock;
			recipeBlock->GetSprite()->ChangeTexture("RecipeSelected");
			});

		previousPosY = recipeBlock->GetPosition().GetY();

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
