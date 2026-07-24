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

void RecipesUi::OnInitialize()
{
	m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;
	GetCollider()->SetIsTrigger(true);
	RecipeDataBase* recipe = RecipeDataBase::Get();
	std::vector<RecipeData*> recipes = recipe->GetAllRecipes();

	Scene* scene = GetScene();
	//Vector2f menuPos = { GetPosition({ 1,0 }).GetX(), GetPosition().GetY() + GetDimension().GetX() * 4.f };
	//creer les differents menus

	//Creer tous les recipes blocks
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

	//Placer les recipesBlocks dans les menus et creer les recipesblocks
	int nextIndexToStart = 0;
	//for (int i = 0; i < recipes.size(); i++)
	while (nextIndexToStart != -1)
	{
		if (nextIndexToStart == -1)
			break;

		RecipesMenu* menu = scene->CreateEntity<RecipesMenu>({}, {400,GetDimension().GetY()- 100}, "", Shape::Rectangle, true);
		menu->GetSprite()->ResizeStretched(menu->GetDimension());
		menu->SetPosition(GetPosition({ 1,1 }) + Vector2f{ -30, 0 }, { 1,1 });
		nextIndexToStart = menu->PlaceRecipeBlock(m_recipes, nextIndexToStart);
		//menu->Initialize(m_recipes, 0);
		//menu->Initialize(recipes, i);
		menu->SetIsVisible(false);
		//i = menu->GetEndRecipeIndex();
		//i = nextIndexToStart;
		m_menus.push_back(menu);
	}

	m_menus[m_actualPageMenu]->SetIsVisible(true);

	m_nextMenuButton = scene->CreateButton({ scene->GetWindowCenter() - Vector2f{0,-200} }, {50,50}, "", "Next", "Next", true);
	m_nextMenuButton->SetOnLeftClick([this] {NextPageMenu(); });
	m_nextMenuButton->GetCollider()->SetIsTrigger(true);

	m_previousMenuButton = scene->CreateButton({ scene->GetWindowCenter() - Vector2f{0,-100} }, { 50,50 }, "", "Previous", "Previous", true);
	m_previousMenuButton->SetOnLeftClick([this] {PreviousPageMenu(); });
	m_previousMenuButton->GetCollider()->SetIsTrigger(true);

	m_craftButton = scene->CreateButton({ scene->GetWindowCenter() - Vector2f{0,-250} }, { 50,50 }, "", "Craft", "Craft", true);
	m_craftButton->SetOnLeftClick([this] {CraftItem(); });
	m_craftButton->GetCollider()->SetIsTrigger(true);

	Vector2f searchPos = GetScene()->GetWindowCenter() + Vector2f{0, -250};
	m_searchItem = scene->CreateTextBox("search", searchPos, { 150, 75 }, ColorsNamed::White, true);
	m_searchItem->GetCollider()->SetIsTrigger(true);
	m_searchItem->SetDrawLayer(INT_MAX);
	m_searchItem->GetText()->SetColor(ColorsNamed::White);
	m_searchItem->SetIsVisible(false);

}

void RecipesUi::OnUpdate()
{
	ChangeSelectedRecipe();
	SortUnlockedRecipe();
	//if (m_searchText != "")
	SortRecipeSearchBox();
}

void RecipesUi::OnEvent()
{
	InputManager* im = InputManager::Get();

	if (im->IsMouseDown(BUTTON_LEFT) && m_searchItem->IsMouseIn())
	{
		im->StartTextInput();
	}
	if (im->IsMouseDown(BUTTON_LEFT) && !m_searchItem->IsMouseIn())
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
		m_searchItem->SetTextInBox(m_searchText);
	}
}

void RecipesUi::Open()
{
	Vector2f pos = GetPosition({ 0,0.5f });

	float offsetY = pos.GetY() + 110;
	m_playerInventory->GetInventory()->SetPosition({pos.GetX() * 1.25f,offsetY}, {0,1});
	m_playerInventory->GetInventory()->SetVisible(true);

	m_playerInventory->GetHotbar()->SetPosition({ pos.GetX() * 1.25f,offsetY + 110 }, { 0,1 });
	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_nextMenuButton->SetIsVisible(true);
	m_previousMenuButton->SetIsVisible(true);
	m_craftButton->SetIsVisible(true);
	m_searchItem->SetIsVisible(true);
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
	m_searchItem->SetIsVisible(false);

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
		c = std::toupper(static_cast<unsigned char>(c));
	}

	std::string textToSearch = prefix + text;

	RecipeDataBase* recipeData = RecipeDataBase::Get();

	std::vector<RecipeBlock*> sortedRecipes;
	std::vector<RecipeBlock*> recipesWithChar;
	std::vector<RecipeBlock*> notSortedRecipes;
	std::vector<RecipeData*>  datas = recipeData->GetRecipeForChar(m_searchText);
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
}

void RecipesUi::ResetSelectedRecipe()
{
	for (RecipeBlock* block : m_recipes)
	{
		block->GetSprite()->ChangeTexture("InventoryWeedyMountain");
	}
	m_actualRecipeBlockSelected = nullptr;
}