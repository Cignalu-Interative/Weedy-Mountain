#pragma once
#include "Button.h"

struct RecipeIngredientData;
class RecipeBlock : public Button
{

private :

	uint32_t m_myID = 0;

public :

	void OnInitialize() override;
	void Initialize(std::string p_recipeName);
};

