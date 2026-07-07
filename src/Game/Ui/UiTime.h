#pragma once
#include "GameData.h"
#include "Ui.h"

class UiTime : public Ui
{
	GameData& m_gameData = GameData::Get();

	void OnInitialize() override;
	void OnUpdate() override;
};

