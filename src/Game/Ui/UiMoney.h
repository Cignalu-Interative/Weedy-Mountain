#pragma once
#include "Ui.h"
#include "GameData.h"

class UiMoney : public Ui
{

	void OnInitialize() override;
	void OnUpdate() override;
	
public :
	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });
};

