#include "pch.h"
#include "RecipesMenu.h"
#include "ItemDataBase.h"
#include "RecipeDataBase.h"

void RecipesMenu::Initialize()
{
	ItemDataBase* item = ItemDataBase::Get();
	RecipeDataBase* recipe = RecipeDataBase::Get();

	recipe->GetAllRecipes();
}

int RecipesMenu::GetLastIndexRecipeAdd()
{

}