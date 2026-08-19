#pragma once
#include "Ui.h"
class Player;

class UiMiner;
class UiFarmer;
class UiGatherer;
class Button;

class UiJobs : public Ui
{
	UiGatherer* m_gathererUi = nullptr;
	UiFarmer* m_farmerUi = nullptr;
	UiMiner* m_minerUi = nullptr;

	Button* m_claimRewardButton = nullptr;

	Player* m_player = nullptr;

	void OnInitialize() override;

	void ClaimReward();

public : 
	void Toggle();
	void SetIsVisible(bool p_visible);

	void Open();
	void Close();
};

