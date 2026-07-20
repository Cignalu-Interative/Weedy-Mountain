#include "pch.h"
#include "RecipeBlock.h"
#include "RecipeDataBase.h"
#include "ItemDataBase.h"
#include "SpriteContainer.h"

static uint32_t id = 1;

void RecipeBlock::OnInitialize()
{
	//RemoveCollider("default");
	GetCollider()->SetIsTrigger(true);
}

void RecipeBlock::Initialize(std::string p_recipeName)
{
	ItemDataBase* item = ItemDataBase::Get();
	RecipeDataBase* recipe = RecipeDataBase::Get();

	ItemData* recipeData = item->GetItem(recipe->GetRecipe(p_recipeName)->GetResultItemID());

	std::string recipePathSprite = recipeData->GetPath();
	std::string recipeName = recipeData->GetName();
	std::string recipeDescription = recipeData->GetDescription();

	AddSprite("Recipe", recipePathSprite, { 50,50 }, 1, - (Vector2f)GetDimension()/2 + Vector2f{ 50, 40 }, true);

	Vector2f spritePos = GetSprite("Recipe")->GetPosition();

	Vector2f recipeNamePos = spritePos + Vector2f{ 75, -10 };
	Debug::Get()->CreateText("RecipeName", recipeNamePos, recipeName, ColorsNamed::White, true);

	Vector2f recipeDescriptionPos = GetSprite("Recipe")->GetPosition({ 1,0.5f }) + Vector2f{ 10, 0};
	Debug::Get()->CreateText("RecipeDescription", {}, recipeDescription, ColorsNamed::White, true);
	Debug::Get()->GetText("RecipeDescription")->SetFontSize(20);
	Debug::Get()->GetText("RecipeDescription")->SetPosition(recipeDescriptionPos, {});


	std::vector<RecipeIngredientData> dataIngredient = recipe->GetRecipe(p_recipeName)->GetIngredients();

	float ingredientOffsetX = 75;

	for (RecipeIngredientData data : dataIngredient)
	{
		Vector2f itemSpritePos = Vector2f{ -(float)GetDimension().GetX() / 2, -(float)GetDimension().GetY() / 1.3f } + Vector2f{ ingredientOffsetX * id ,150 };
		AddSprite(std::to_string(m_myID + id), item->GetItem(data.itemID)->GetPath(), { 50,50 }, 1, itemSpritePos, true);
		
		Vector2f spritePos = GetSprite(std::to_string(m_myID + id))->GetPosition();
		Vector2f textQuantityPos = spritePos + Vector2f{ 20,10 };

		Debug::Get()->CreateText(std::to_string(m_myID + id), textQuantityPos, std::to_string(data.quantity), ColorsNamed::White, true);
		id++;
	}
}
