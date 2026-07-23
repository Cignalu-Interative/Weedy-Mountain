#pragma once
#include "Entity.h"
#include "RecipeBlock.h"

class RecipesMenu;
class Button;
class PlayerInventory;
//class RecipeBlock;
class RecipesUi : public Entity
{
private:
	std::vector<RecipeBlock*> m_recipes;
	std::vector<RecipesMenu*> m_menus;
	int m_actualPageMenu = 0; // actual page index

	PlayerInventory* m_playerInventory = nullptr;

	Button* m_craftButton = nullptr;
	Button* m_nextMenuButton = nullptr;
	Button* m_previousMenuButton = nullptr;

	RecipeBlock* m_lastRecipeBlockSelected = nullptr;
	RecipeBlock* m_actualRecipeBlockSelected = nullptr;

	void OnInitialize() override;
	void OnUpdate() override;

	void NextPageMenu();
	void PreviousPageMenu();

	void CraftItem();

	void ChangeSelectedRecipe()
	{
		for (RecipeBlock* block : m_recipes)
		{
			if (block != m_actualRecipeBlockSelected && block->GetIsUnlocked())
				block->GetSprite()->ChangeTexture("InventoryWeedyMountain");
		}
	}

	RecipeBlock* GetSelectedRecipe() { return m_actualRecipeBlockSelected; }
public :

	void Open();
	void Close();

	void SortUnlockedRecipe(); // put unlocked recipes in top of menu

	void ResetSelectedRecipe()
	{
		for (RecipeBlock* block : m_recipes)
		{
			block->GetSprite()->ChangeTexture("InventoryWeedyMountain");
		}
	}
};

