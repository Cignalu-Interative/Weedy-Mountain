#include "pch.h"
#include "GameData.h"
#include "RecipesUi.h"
#include "RecipeBlock.h"
#include "RecipesMenu.h"
#include "RecipeDataBase.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "RecipeData.h"
#include "2DPendentLib/InputManager.h"
#include <algorithm>

void RecipesUi::OnInitialize()
{
	m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;
	GetCollider()->SetIsTrigger(true);
	RecipeDataBase* recipe = RecipeDataBase::Get();
	std::vector<RecipeData*> recipes = recipe->GetAllRecipes();
	Scene* scene = GetScene();
	//Vector2f menuPos = { GetPosition({ 1,0 }).GetX(), GetPosition().GetY() + GetDimension().GetX() * 4.f };

	for (int i = 0; i < recipes.size(); i++)
	{
		RecipeBlock* recipeBlock = GetScene()->CreateEntity<RecipeBlock>({}, { 400,150 }, "InventoryWeedyMountain", Shape::Rectangle, true);
		recipeBlock->Initialize(recipes[i]->GetRecipeID());
		recipeBlock->SetOnLeftClick([this, recipeBlock] {
			m_lastRecipeBlockSelected = m_actualRecipeBlockSelected;
			m_actualRecipeBlockSelected = recipeBlock;
			recipeBlock->GetSprite()->ChangeTexture("RecipeSelected");
			});

		m_recipes.push_back(recipeBlock);
	}


	int nextIndexToStart = 0;
	while (nextIndexToStart != -1)
	{
		if (nextIndexToStart == -1)
			break;

		RecipesMenu* menu = scene->CreateEntity<RecipesMenu>({}, {400,GetDimension().GetY()- 100}, "InventoryWeedyMountain", Shape::Rectangle, true);
		menu->SetDrawLayer(INT_MAX - 100);
		menu->GetSprite()->ResizeStretched(menu->GetDimension());
		menu->SetPosition(GetPosition() + Vector2f{ +325, 0 });
		nextIndexToStart = menu->PlaceRecipeBlock(m_recipes, nextIndexToStart);
		//menu->Initialize(m_recipes, 0);
		//menu->Initialize(recipes, i);
		menu->SetIsVisible(false);
		//i = menu->GetEndRecipeIndex();
		//i = nextIndexToStart;
		m_menus.push_back(menu);
	}
	if (m_menus.empty())
		return;
	
	m_menus[m_actualPageMenu]->SetIsVisible(true);

	Vector2f nextButtonPos = m_menus[0]->GetPosition({ 1,1 }) + Vector2f{ -50, 20 };
	m_nextMenuButton = scene->CreateButton({ nextButtonPos }, { 80,50 }, "InventoryWeedyMountain", "Next", "Next", true);
	m_nextMenuButton->SetOnLeftClick([this] {NextPageMenu(); });
	m_nextMenuButton->GetCollider()->SetIsTrigger(true);

	Vector2f previousButtonPos = m_menus[0]->GetPosition({ 0,1 }) + Vector2f{ 50, 20 };
	m_previousMenuButton = scene->CreateButton({ previousButtonPos }, {100,50}, "InventoryWeedyMountain", "Previous", "Previous", true);
	m_previousMenuButton->SetOnLeftClick([this] {PreviousPageMenu(); });
	m_previousMenuButton->GetCollider()->SetIsTrigger(true);

	Vector2f craftButtonPos = m_menus[0]->GetPosition({ 0.5,1 }) + Vector2f{ 0, 20 };
	m_craftButton = scene->CreateButton({ craftButtonPos }, { 80,50 }, "InventoryWeedyMountain", "Craft", "Craft", true);
	m_craftButton->SetOnLeftClick([this] {CraftItem(); });
	m_craftButton->GetCollider()->SetIsTrigger(true);

	Vector2f textBoxPos = m_menus[0]->GetPosition({ 0.5,0 }) - Vector2f{0, 20};
	m_textBox = scene->CreateTextBox("search", textBoxPos, { 150, 40 }, ColorsNamed::White, true);
	m_textBox->GetCollider()->SetIsTrigger(true);
	m_textBox->SetDrawLayer(INT_MAX);
	m_textBox->GetText()->SetColor(ColorsNamed::White);
	m_textBox->SetIsVisible(false);

}

void RecipesUi::OnUpdate()
{
	ChangeSelectedRecipe();
	SortUnlockedRecipe();
	SortRecipeSearchBox();
}

void RecipesUi::OnEvent()
{
	InputManager* im = InputManager::Get();

	if (im->IsMouseDown(BUTTON_LEFT) && m_textBox->IsMouseIn())
	{
		im->StartTextInput();
	}
	if (im->IsMouseDown(BUTTON_LEFT) && !m_textBox->IsMouseIn())
	{
		im->StopTextInput();
	}
	if (im->IsTextInputActive() == true && im->IsKeyboardDown(K_DELETE))
	{
		im->DeleteLastCharInFinalTextInput();
	}
	if (im->IsTextInputActive() == true)
	{
		m_searchText = im->GetfinalTextInput();
		m_textBox->SetTextInBox(m_searchText);
	}
}

void RecipesUi::Open()
{
	Vector2f pos = GetPosition({ 0,0.5f });

	float offsetY = pos.GetY() + 80;
	m_playerInventory->GetInventory()->SetPosition({pos.GetX() * 1.25f,offsetY}, {0,1});
	m_playerInventory->GetInventory()->SetVisible(true);

	m_playerInventory->GetHotbar()->SetPosition({ pos.GetX() * 1.25f,offsetY + 110 }, { 0,1 });
	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_nextMenuButton->SetIsVisible(true);
	m_previousMenuButton->SetIsVisible(true);
	m_craftButton->SetIsVisible(true);
	m_textBox->SetIsVisible(true);
	//m_actualPageMenu = 0;

	//for(RecipesMenu* menu : m_menus)
	//{
	//	menu->SetIsVisible(false);
	//	menu->ResetSelectedRecipe();	
	//}

	m_menus[m_actualPageMenu]->SetIsVisible(true);
}

void RecipesUi::Close()
{
	SetIsVisible(false);
	
	Vector2f hotbarPos = GetScene()->GetWindowCenter() + Vector2f{ 0, 300 };
	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());
	m_playerInventory->GetInventory()->SetVisible(false);

	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	m_nextMenuButton->SetIsVisible(false);
	m_previousMenuButton->SetIsVisible(false);
	m_craftButton->SetIsVisible(false);
	m_textBox->SetIsVisible(false);

	for (RecipesMenu* menu : m_menus)
	{
		menu->SetIsVisible(false);
	}
}

void RecipesUi::NextPageMenu()
{
	m_menus[m_actualPageMenu]->SetIsVisible(false);
	ResetSelectedRecipe();

	m_actualPageMenu++;

	int size = m_menus.size()- 1;
	if (m_actualPageMenu > size)
		m_actualPageMenu = size;

	m_menus[m_actualPageMenu]->SetIsVisible(true);
}

void RecipesUi::PreviousPageMenu()
{
	m_menus[m_actualPageMenu]->SetIsVisible(false);
	ResetSelectedRecipe();

	m_actualPageMenu--;

	if (m_actualPageMenu < 0)
		m_actualPageMenu = 0;

	m_menus[m_actualPageMenu]->SetIsVisible(true);
}

void RecipesUi::CraftItem()
{	
	RecipeBlock* selectedRecipe = GetSelectedRecipe();
	if (selectedRecipe == nullptr)
		return;

	if (selectedRecipe->GetIsUnlocked() == false)
		return;

	bool canCraft = false;

	for (RecipeIngredientData data : selectedRecipe->GetIngredients())
	{
		if (m_playerInventory->HasItems(data.itemID, data.quantity))
			canCraft = true;
		else
			canCraft = false;
	}

	if (canCraft)
	{
		m_playerInventory->AddItem(selectedRecipe->GetItemName(), selectedRecipe->GetAmountItem(), false);
		for (RecipeIngredientData data : selectedRecipe->GetIngredients())
		{
			m_playerInventory->RemoveItem(data.itemID, data.quantity, false);
		}
	}

}

void RecipesUi::ChangeSelectedRecipe()
{
	for (RecipeBlock* block : m_recipes)
	{
		if (block != m_actualRecipeBlockSelected && block->GetIsUnlocked())
			block->GetSprite()->ChangeTexture("InventoryWeedyMountain");
	}
}

void RecipesUi::SortUnlockedRecipe()
{
	std::vector <RecipeBlock*> unlockRecipes;
	std::vector <RecipeBlock*> lockRecipes;
	std::vector <RecipeBlock*> sortedRecipes;
	// Sort all recipes in all pages

	//Sort unlocked and locked recipes in all pages
	for (RecipesMenu* menu : m_menus)
	{
		std::vector <RecipeBlock*> blocks = menu->GetRecipeBlock();
		for (RecipeBlock* block : blocks)
		{
			if (block->GetIsUnlocked())
				unlockRecipes.push_back(block);
			else
				lockRecipes.push_back(block);
		}
	}

	if(unlockRecipes.empty())
		return;

	sortedRecipes = unlockRecipes;
	sortedRecipes.insert(sortedRecipes.end(), lockRecipes.begin(), lockRecipes.end());

	int nextIndexToStart = 0;
	for (RecipesMenu* menu : m_menus)
	{
		nextIndexToStart = menu->PlaceRecipeBlock(sortedRecipes, nextIndexToStart);
	}

	unlockRecipes.clear();
	lockRecipes.clear();
	sortedRecipes.clear();

	for (RecipesMenu* menu : m_menus)
	{
		menu->SetIsVisible(false);
	}

	m_menus[m_actualPageMenu]->SetIsVisible(true);
}

void RecipesUi::SortRecipeSearchBox()
{

	if (m_searchText.empty())
		return;


	std::string prefix = "ITEM_";
	std::string text = m_searchText;
	for (char& c : text) {
		c = std::toupper(unsigned char(c));
	}

	std::string textToSearch = prefix + text;
	std::replace(textToSearch.begin(), textToSearch.end(), ' ', '_');

	RecipeDataBase* recipeData = RecipeDataBase::Get();

	std::vector<RecipeBlock*> sortedRecipes;
	std::vector<RecipeBlock*> recipesWithChar;
	std::vector<RecipeBlock*> notSortedRecipes;
	std::vector<RecipeData*>  datas = recipeData->GetRecipeForChar(textToSearch);
	std::vector<RecipeData*>  recipe = recipeData->GetRecipesForItem(textToSearch);

	//if (datas.empty())
	//	return;

	for (RecipesMenu* menu : m_menus)
	{
		std::vector <RecipeBlock*> blocks = menu->GetRecipeBlock();
		for (RecipeBlock* block : blocks)
		{
			for (RecipeData* data : recipe)
			{
				if (block->GetItemName() == data->GetResultItemID())
					recipesWithChar.push_back(block);
				else
					notSortedRecipes.push_back(block);
			}
		}
	}
	if (recipesWithChar.empty())
		return;

	sortedRecipes = recipesWithChar;
	sortedRecipes.insert(sortedRecipes.end(), notSortedRecipes.begin(), notSortedRecipes.end());

	int nextIndexToStart = 0;
	for (RecipesMenu* menu : m_menus)
	{
		nextIndexToStart = menu->PlaceRecipeBlock(sortedRecipes, nextIndexToStart);
	}

	recipesWithChar.clear();
	notSortedRecipes.clear();
	sortedRecipes.clear();

	for (RecipesMenu* menu : m_menus)
	{
		menu->SetIsVisible(false);
	}
	m_menus[m_actualPageMenu]->SetIsVisible(true);
}

void RecipesUi::ResetSelectedRecipe()
{
	for (RecipeBlock* block : m_recipes)
	{
		block->GetSprite()->ChangeTexture("InventoryWeedyMountain");
	}
	m_actualRecipeBlockSelected = nullptr;
}

