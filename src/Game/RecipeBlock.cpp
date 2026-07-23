#include "pch.h"
#include "RecipeBlock.h"
#include "RecipeDataBase.h"
#include "ItemDataBase.h"
#include "GameData.h"
#include "SpriteContainer.h"

static uint32_t id = 0;

RecipeBlock::RecipeBlock()
{
	m_myID = id++;
}

void RecipeBlock::OnInitialize()
{
	//RemoveCollider("default");
	GetCollider()->SetIsTrigger(true);
}

void RecipeBlock::OnUpdate()
{
	Button::OnUpdate();

	if (m_isUnlocked)
		return;

	bool unlocked = GameData::Get().IsRecipeUnlocked(m_recipeName);
	if (unlocked)
	{
		m_isUnlocked = true;
		RemoveSprite("LockedRecipe");
		GetSprite()->ChangeTexture("InventoryWeedyMountain");
	}
	else
		GetSprite()->ChangeTexture("LockedRecipe");
}
void RecipeBlock::Initialize(std::string p_recipeName)
{
	ItemDataBase* item = ItemDataBase::Get();
	RecipeDataBase* recipe = RecipeDataBase::Get();
	Debug* debug = Debug::Get();

	ItemData* recipeData = item->GetItem(recipe->GetRecipe(p_recipeName)->GetResultItemID());

	m_recipeName = p_recipeName;
	m_resultItemName = recipe->GetRecipe(p_recipeName)->GetResultItemID();
	m_amountItem = recipe->GetRecipe(p_recipeName)->GetResultQuantity();

	std::string recipePathSprite = recipeData->GetPath();
	std::string recipeName = recipeData->GetName();
	std::string recipeDescription = recipeData->GetDescription();

	AddSprite("LockedRecipe", "LockedRecipe", GetDimension(), 0, {}, true);
	AddSprite("Recipe", recipePathSprite, { 50,50 }, 1, - (Vector2f)GetDimension()/2 + Vector2f{ 50, 40 }, true);

	Vector2f spritePos = GetSprite("Recipe")->GetPosition();

	Vector2f recipeNamePos = spritePos + Vector2f{ 75, -10 };
	debug->CreateText("RecipeName" + std::to_string(m_myID), recipeNamePos, recipeName, ColorsNamed::White, true);

	Vector2f recipeDescriptionPos = GetSprite("Recipe")->GetPosition({ 1,0.5f }) + Vector2f{ 10, 0};
	debug->CreateText("RecipeDescription" + std::to_string(m_myID), {}, recipeDescription, ColorsNamed::White, true);
	debug->GetText("RecipeDescription" + std::to_string(m_myID))->SetFontSize(20);
	debug->GetText("RecipeDescription" + std::to_string(m_myID))->SetPosition(recipeDescriptionPos, {});

	m_ingredients = recipe->GetRecipe(p_recipeName)->GetIngredients();

	float ingredientOffsetX = 75;
	int number = 1;
	for (RecipeIngredientData data : m_ingredients)
	{
		Vector2f itemSpritePos = Vector2f{ -(float)GetDimension().GetX() / 2, -(float)GetDimension().GetY() / 1.3f } + Vector2f{ ingredientOffsetX * number,150 };
		AddSprite(std::to_string(m_myID + number), item->GetItem(data.itemID)->GetPath(), { 50,50 }, 1, itemSpritePos, true);

		Vector2f spritePos = GetSprite(std::to_string(m_myID + number))->GetPosition();
		Vector2f textQuantityPos = spritePos + Vector2f{ 25,10 };

		debug->CreateText(std::to_string(m_myID) + "_amount_" + std::to_string(number), textQuantityPos, std::to_string(data.quantity), ColorsNamed::White, true);
		number++;
	}
}

void RecipeBlock::SetIsVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);
	Debug::Get()->GetText("RecipeName" + std::to_string(m_myID))->SetIsVisible(p_isVisible);
	Debug::Get()->GetText("RecipeDescription" + std::to_string(m_myID))->SetIsVisible(p_isVisible);

	int number = 1;
	for (RecipeIngredientData data : m_ingredients)
	{
		GetSprite(std::to_string(m_myID + number))->SetIsVisible(p_isVisible);
		Debug::Get()->GetText(std::to_string(m_myID) + "_amount_" + std::to_string(number))->SetIsVisible(p_isVisible);

		number++;
	}
}

void RecipeBlock::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Debug* debug = Debug::Get();

	Entity::SetPosition(p_position, p_anchor);

	Vector2f recipeDescriptionPos = GetSprite("Recipe")->GetPosition({ 1,0.5f }) + Vector2f{ 10, 0 };
	debug->GetText("RecipeDescription" + std::to_string(m_myID))->SetPosition(recipeDescriptionPos, {});

	int number = 1;
	for (RecipeIngredientData data : m_ingredients)
	{
		Vector2f spritePos = GetSprite(std::to_string(m_myID + number))->GetPosition();
		Vector2f textQuantityPos = spritePos + Vector2f{ 25,10 };

		Debug::Get()->GetText(std::to_string(m_myID) + "_amount_" + std::to_string(number))->SetPosition(textQuantityPos);
		number++;
	}

	Vector2f spritePos = GetSprite("Recipe")->GetPosition();
	Vector2f recipeNamePos = spritePos + Vector2f{ 75, -10 };
	debug->GetText("RecipeName" + std::to_string(m_myID))->SetPosition(recipeNamePos);
}