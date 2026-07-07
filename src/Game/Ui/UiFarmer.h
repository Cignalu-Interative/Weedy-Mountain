#pragma once
#include "Ui.h"
class Player;
class UiFarmer : public Ui
{
	Player* m_player = nullptr;

	void OnInitialize() override;
	void OnUpdate() override;
};

