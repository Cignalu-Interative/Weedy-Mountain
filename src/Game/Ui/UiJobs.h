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

	bool m_isOpen = false;

public : 
	void Toggle();
	void SetIsVisible(bool p_visible);

	void Open();
	void Close();

	void SetPosition(Vector2f p_position, Vector2f p_anchor = {0.5f, 0.5f});

	bool GetIsOpen() { return m_isOpen; }
};

