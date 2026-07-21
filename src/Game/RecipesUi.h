#pragma once
#include "Entity.h"

class RecipesMenu;
class Button;
class PlayerInventory;
class RecipesUi : public Entity
{
private:

	std::vector<RecipesMenu*> m_menus;
	int m_actualPageMenu = 0; // actual page index

	PlayerInventory* m_playerInventory = nullptr;

	Button* m_craftButton = nullptr;
	Button* m_nextMenuButton = nullptr;
	Button* m_previousMenuButton = nullptr;

	void OnInitialize() override;

	void NextPageMenu();
	void PreviousPageMenu();

	void CraftItem();


	void SortUnlockedRecipe(); // put unlocked recipes in top of menu

public :

	void Open();
	void Close();
};

