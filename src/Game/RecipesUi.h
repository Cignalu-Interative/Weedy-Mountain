#pragma once
#include "Entity.h"
#include "TextBox.h"

class RecipesMenu;
class Button;
class PlayerInventory;
class RecipeBlock;
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

	TextBox* m_textBox = nullptr;
	std::string m_searchText;

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;

	void NextPageMenu();
	void PreviousPageMenu();

	void CraftItem();

	void ChangeSelectedRecipe();

	RecipeBlock* GetSelectedRecipe() { return m_actualRecipeBlockSelected; }

	void SortUnlockedRecipe(); // put unlocked recipes in top of menu

	void SortRecipeSearchBox();

	void ResetSelectedRecipe();

public :

	void Open();
	void Close();

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });

};

