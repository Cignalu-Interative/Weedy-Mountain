#pragma once
#include "Ui.h"
#include "GameData.h"

class UiDate : public Ui
{

	GameData& m_gameData = GameData::Get();

	void OnInitialize() override;
	void OnUpdate() override;
};

