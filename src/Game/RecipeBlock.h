#pragma once
#include "Button.h"

struct RecipeIngredientData;
class RecipeBlock : public Button
{

private :

	uint32_t m_myID = 0;

	std::string m_recipeName;
	std::string m_resultItemName;
	int m_amountItem = 0;

	std::vector<RecipeIngredientData> m_ingredients;

	bool m_isUnlocked = false;

	void OnInitialize() override;
	void OnUpdate() override;

public :
	RecipeBlock();

	void Initialize(std::string p_recipeName);
	void SetIsVisible(bool p_isVisible);

	std::vector<RecipeIngredientData> GetIngredients() { return m_ingredients; }
	bool GetIsSelected() const { return m_isUnlocked; }

	const std::string& GetItemName() { return m_resultItemName; }
	int GetAmountItem() const{ return m_amountItem; }
	std::string GetRecipeName() const { return m_recipeName; }
};



