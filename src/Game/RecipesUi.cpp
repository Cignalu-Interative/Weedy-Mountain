#include "pch.h"
#include "GameData.h"
#include "RecipesUi.h"
#include "RecipeBlock.h"
#include "RecipesMenu.h"
#include "RecipeDataBase.h"
void RecipesUi::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);
	RecipeDataBase* recipe = RecipeDataBase::Get();
	std::vector<RecipeData*> recipes = recipe->GetAllRecipes();

	//Vector2f menuPos = { GetPosition({ 1,0 }).GetX(), GetPosition().GetY() + GetDimension().GetX() * 4.f };
	//creer les differents menus
	for (int i = 0; i < recipes.size(); i++)
	{
		RecipesMenu* menu = GetScene()->CreateEntity<RecipesMenu>({}, {400,GetDimension().GetY()- 100}, "InventoryWeedyMountain", Shape::Rectangle, true);
		menu->GetSprite()->ResizeStretched(menu->GetDimension());
		menu->SetPosition(GetPosition({ 1,1 }) + Vector2f{ -30, 0 }, { 1,1 });
		menu->Initialize(recipes, i);
		menu->SetIsVisible(false);
		i = menu->GetEndRecipeIndex();

		m_menus.push_back(menu);
	}

	m_menus[m_actualPageMenu]->SetIsVisible(true);
}

void RecipesUi::Open()
{
	//Inventory placement
	Vector2f pos = GetPosition({ 0,0.5f });

	float offsetY = pos.GetY() + 110;
	//m_playerInventory->GetInventory()->SetPosition({pos.GetX() * 1.25f,offsetY}, {0,1});
	//m_playerInventory->GetInventory()->SetVisible(true);

	//m_playerInventory->GetHotbar()->SetPosition({ pos.GetX() * 1.25f,offsetY + 110 }, { 0,1 });
	//m_playerInventory->GetHotbar()->SetIsVisible(true);

}

void RecipesUi::Close()
{
	Vector2f hotbarPos = GetScene()->GetWindowCenter() + Vector2f{ 0, 300 };
	SetIsVisible(false);

	//m_playerInventory->GetHotbar()->SetIsVisible(true);
	//m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	//m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());
}

void RecipesUi::NextPageMenu()
{

}

void RecipesUi::PreviousPageMenu()
{

}

void RecipesUi::SortUnlockedRecipe()
{

}
