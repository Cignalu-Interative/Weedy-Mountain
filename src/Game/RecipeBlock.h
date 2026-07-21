#pragma once
#include "Button.h"

struct RecipeIngredientData;
class RecipeBlock : public Button
{

private :

	uint32_t m_myID = 0;

	std::string m_itemName;
	int m_amountItem = 0;

	std::vector<RecipeIngredientData> m_ingredients;

	bool m_isSelected = false;

	void OnInitialize() override;
	void OnUpdate() override;
public :
	RecipeBlock();

	void Initialize(std::string p_recipeName);
	void SetIsVisible(bool p_isVisible);

	std::vector<RecipeIngredientData> GetIngredients() { return m_ingredients; }
	bool GetIsSelected() { return m_isSelected; }
	void SetIsSelected(bool p_isSelected) { m_isSelected = p_isSelected; }

	const std::string& GetItemName() { return m_itemName; }
	int GetAmountItem() { return m_amountItem; }
};



