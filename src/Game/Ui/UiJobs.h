#pragma once
#include "Ui.h"
class Player;

class UiMiner;
class UiFarmer;
class UiGatherer;

class UiJobs : public Ui
{
	UiGatherer* m_gathererUi = nullptr;
	UiFarmer* m_farmerUi = nullptr;
	UiMiner* m_minerUi = nullptr;

	Player* m_player = nullptr;

	void OnInitialize() override;

	void SetIsVisible(bool p_isVisible);

};

