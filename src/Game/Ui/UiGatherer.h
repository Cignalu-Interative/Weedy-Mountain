#pragma once
#include "Ui.h"

class Player;
class UiGatherer : public Ui
{
	Player* m_player = nullptr;
	GameData& gm = GameData::Get();

	void OnInitialize() override;
	void OnUpdate() override;

public:

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });
};

