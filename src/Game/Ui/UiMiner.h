#pragma once
#include "Ui.h"
class Player;
class UiMiner : public Ui
{
	Player* m_player = nullptr;
	GameData& gm = GameData::Get();

	void OnInitialize() override;
	void OnUpdate() override;
};



