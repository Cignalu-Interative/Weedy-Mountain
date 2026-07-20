#pragma once
#include "Button.h"

struct RecipeIngredientData;
class RecipeBlock : public Button
{

private :

	uint32_t m_myID = 0;

	std::vector<RecipeIngredientData> m_ingredients;
public :
	RecipeBlock();
	~RecipeBlock() {
		Debug::Get()->destroyText("RecipeName" + std::to_string(m_myID));
		Debug::Get()->destroyText("RecipeDescription" + std::to_string(m_myID));
		for (int i = 1; i < 10; i++)
		{
			Debug::Get()->destroyText(std::to_string(m_myID + i));
		}
	}
	void OnInitialize() override;
	void Initialize(std::string p_recipeName);
	void SetIsVisible(bool p_isVisible);
};

