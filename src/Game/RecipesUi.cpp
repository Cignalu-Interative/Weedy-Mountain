#include "pch.h"
#include "GameData.h"
#include "RecipesUi.h"
#include "RecipeBlock.h"
#include "RecipesMenu.h"
#include "RecipeDataBase.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "RecipeData.h"

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
}

void RecipesUi::OnUpdate()
{
	ChangeSelectedRecipe();
	SortUnlockedRecipe();
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

	for (RecipesMenu* menu : m_menus)
	{
		menu->SetIsVisible(false);
	}
}

void RecipesUi::NextPageMenu()
{
	m_menus[m_actualPageMenu]->SetIsVisible(false);
	//m_menus[m_actualPageMenu]->ResetSelectedRecipe();
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
	//m_menus[m_actualPageMenu]->ResetSelectedRecipe();
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

	// Passer dans la boucle pour delete les pointeurs
	unlockRecipes.clear();
	lockRecipes.clear();
	sortedRecipes.clear();

	for (RecipesMenu* menu : m_menus)
	{
		menu->SetIsVisible(false);
	}

	m_menus[m_actualPageMenu]->SetIsVisible(true);
}