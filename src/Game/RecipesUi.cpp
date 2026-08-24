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
#include "SpriteContainer.h"
#include "HUDScene.h"
#include <algorithm>

void RecipesUi::OnInitialize()
{
	AddSprite("RecipeUiBackground", "Crafting_BackGround", GetDimension(), 0, {}, true);
	GetSprite("RecipeUiBackground")->ResizeStretched(GetDimension());
	SyncSprites();

	m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;
	GetCollider()->SetIsTrigger(true);
	RecipeDataBase* recipe = RecipeDataBase::Get();
	std::vector<RecipeData*> recipes = recipe->GetAllRecipes();
	Scene* scene = GetScene();
	Vector2f titlePos = GetPosition({}) + Vector2f{125, 50};
	Debug::Get()->CreateText("CraftTitle", titlePos, "CRAFT", ColorsNamed::White, true);
	Debug::Get()->GetText("CraftTitle")->SetFontSize(100);
	Debug::Get()->GetText("CraftTitle")->SetPosition(titlePos);

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
		menu->SetPosition(GetPosition() + Vector2f{ 325, 100 });
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
	m_nextMenuButton = scene->CreateButton({ nextButtonPos }, { 80,70 }, "button", "Next", "Next", true);
	m_nextMenuButton->SetOnLeftClick([this] {NextPageMenu(); });
	m_nextMenuButton->GetCollider()->SetIsTrigger(true);
	m_nextMenuButton->SetOnHoverStay([this] { m_nextMenuButton->SetSprite("button2"); });
	m_nextMenuButton->SetOnHoverExit([this] { m_nextMenuButton->SetSprite("button"); });


	Vector2f previousButtonPos = m_menus[0]->GetPosition({ 0,1 }) + Vector2f{ 50, 20 };
	m_previousMenuButton = scene->CreateButton({ previousButtonPos }, {90,70}, "button", "Previous", "Previous", true);
	m_previousMenuButton->SetOnLeftClick([this] {PreviousPageMenu(); });
	m_previousMenuButton->GetCollider()->SetIsTrigger(true);
	m_previousMenuButton->SetOnHoverStay([this] { m_previousMenuButton->SetSprite("button2"); });
	m_previousMenuButton->SetOnHoverExit([this] { m_previousMenuButton->SetSprite("button"); });

	Vector2f craftButtonPos = m_menus[0]->GetPosition({ 0.5,1 }) + Vector2f{ 0, 20 };
	m_craftButton = scene->CreateButton({ craftButtonPos }, { 80,70 }, "button", "Craft", "Craft", true);
	m_craftButton->SetOnLeftClick([this] {CraftItem(); });
	m_craftButton->GetCollider()->SetIsTrigger(true);
	m_craftButton->SetOnHoverStay([this] { m_craftButton->SetSprite("button2"); });
	m_craftButton->SetOnHoverExit([this] { m_craftButton->SetSprite("button"); });

	Vector2f textBoxPos = GetPosition() -Vector2f{200,180 };//m_menus[0]->GetPosition({ 0.5,0 }) - Vector2f{0, 20};
	m_textBox = scene->CreateTextBox("search", textBoxPos, { 150, 40 }, ColorsNamed::Transparent, true);
	m_textBox->GetCollider()->SetIsTrigger(true);
	m_textBox->SetDrawLayer(INT_MAX);
	m_textBox->GetText()->SetColor(ColorsNamed::White);
	m_textBox->SetIsVisible(false);

	AddSprite("LoupeIcon", "Search_Bar", { 40,40}, 1, {-Vector2f{290,180}}, true);

	m_textBoxBackground = GetScene()->CreateEntity<Entity>(m_textBox->GetPosition(), m_textBox->GetDimension(), "Search_Bar2", Shape::Rectangle,true);
	m_textBoxBackground->GetSprite()->ResizeStretched(m_textBoxBackground->GetDimension());
	m_textBoxBackground->GetCollider()->SetIsTrigger(true);
	m_textBoxBackground->SetDrawLayer(m_textBox->GetDrawLayer() - 1);
	m_textBoxBackground->SyncSprites(); 
}

void RecipesUi::OnUpdate()
{
	if (m_isVisible == false)
		return;
	
	if(m_actualRecipeBlockSelected != nullptr)
	{
		if (m_actualRecipeBlockSelected->GetIsUnlocked())
			m_craftButton->SetIsVisible(true);
	}
		
	m_craftButton->SetIsVisible(false);
	
	ChangeSelectedRecipe();
	SortUnlockedRecipe();
	SortRecipeSearchBox();
}

void RecipesUi::OnEvent()
{
	if(m_isVisible == false)
		return;

	InputManager* im = InputManager::Get();

	if (im->IsMouseDown(BUTTON_LEFT) && m_textBox->IsMouseIn())
	{
		im->StartTextInput();
	}
	if (im->IsMouseDown(BUTTON_LEFT) && !m_textBox->IsMouseIn())
	{
		im->StopTextInput();
	}
	if (im->IsTextInputActive() == true && im->IsKeyboardDown(K_BACKSPACE))
	{
		im->DeleteLastCharInFinalTextInput();
	}
	if (im->IsTextInputActive() == true)
	{
		m_searchText = im->GetfinalTextInput();
		m_textBox->SetTextInBox(m_searchText);
	}
	if (im->IsKeyboardDown(K_ESCAPE))
	{
		Close();
	}
}

void RecipesUi::Open()
{
	GetScene<HUDScene>()->HideUi();

	Vector2f pos = GetPosition({ 0,0.5f });
	
	float offsetY = pos.GetY() + 80;
	m_playerInventory->GetInventory()->SetPosition({pos.GetX() + 70,offsetY}, {0,1});
	m_playerInventory->GetInventory()->SetVisible(true);

	m_playerInventory->GetHotbar()->SetPosition({ pos.GetX() + 70,offsetY + 110 }, { 0,1 });
	m_playerInventory->GetHotbar()->SetIsVisible(true);

	SetIsVisible(true);
	m_nextMenuButton->SetIsVisible(true);
	m_previousMenuButton->SetIsVisible(true);
	m_craftButton->SetIsVisible(true);
	m_textBox->SetIsVisible(true);
	m_textBoxBackground->SetIsVisible(true);
	m_menus[m_actualPageMenu]->SetIsVisible(true);
	Debug::Get()->GetText("CraftTitle")->SetIsVisible(true);

	m_isOpen = true;
}

void RecipesUi::Close()
{
	if (m_isVisible)
		GetScene<HUDScene>()->ShowUi();

	SetIsVisible(false);
	
	Vector2f hotbarPos = { GetScene()->GetWindowCenter().GetX(),(float)GetScene()->GetWindowSize().GetY() - 12 };

	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());
	m_playerInventory->GetInventory()->SetVisible(false);

	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos, {0.5f, 1.0f});

	m_nextMenuButton->SetIsVisible(false);
	m_previousMenuButton->SetIsVisible(false);
	m_craftButton->SetIsVisible(false);
	m_textBox->SetIsVisible(false);
	m_textBoxBackground->SetIsVisible(false);

	Debug::Get()->GetText("CraftTitle")->SetIsVisible(false);
	for (RecipesMenu* menu : m_menus)
	{
		menu->SetIsVisible(false);
	}
	m_isOpen = false;
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

	for (char& c : m_searchText) 
	{
		c = std::toupper(unsigned char(c));
	}

	std::replace(m_searchText.begin(), m_searchText.end(), ' ', '_');

	RecipeDataBase* recipeData = RecipeDataBase::Get();

	std::vector<RecipeBlock*> sortedRecipes;
	std::vector<RecipeBlock*> recipesWithChar;
	std::vector<RecipeBlock*> notSortedRecipes;
	std::vector<RecipeData*>  datas = recipeData->GetRecipeForChar(m_searchText);

	if (datas.empty())
		return;

	for (RecipesMenu* menu : m_menus)
	{
		std::vector <RecipeBlock*> blocks = menu->GetRecipeBlock();
		for (RecipeBlock* block : blocks)
		{
			bool match = false;

			for (RecipeData* data : datas)
			{
				if (block->GetItemName() == data->GetResultItemID())
				{
					match = true;
					break;
				}
			}

			if(match)
				recipesWithChar.push_back(block);
			else
				notSortedRecipes.push_back(block);
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

void RecipesUi::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);

	int nextIndexToStart = 0;
	for (RecipesMenu* menu : m_menus)
	{
		menu->SetPosition(GetPosition() + Vector2f{ +325, -25 });
		nextIndexToStart = menu->PlaceRecipeBlock(m_recipes, nextIndexToStart);
	}

	Vector2f nextButtonPos = m_menus[0]->GetPosition({ 1,1 }) + Vector2f{ -50, 30 };
	Vector2f previousButtonPos = m_menus[0]->GetPosition({ 0,1 }) + Vector2f{ 50, 30 };
	Vector2f craftButtonPos = m_menus[0]->GetPosition({ 0.5,1 }) + Vector2f{ 0, 30 };
	Vector2f textBoxPos = GetPosition() - Vector2f{ 200,180 };//m_menus[0]->GetPosition({ 0.5,0 }) - Vector2f{ 0, 20 };

	m_nextMenuButton->SetPosition(nextButtonPos);
	m_previousMenuButton->SetPosition(previousButtonPos);
	m_craftButton->SetPosition(craftButtonPos);
	m_textBox->SetPosition(textBoxPos);
	m_textBoxBackground->SetPosition(m_textBox->GetPosition());
	
	Debug::Get()->GetText("CraftTitle")->SetPosition(GetPosition({}) + Vector2f{ 125, 50 });
}