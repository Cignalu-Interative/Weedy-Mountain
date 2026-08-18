#pragma once
#include "Ui.h"

class UiLife : public Ui
{
public :
	void OnInitialize() override;
	void OnUpdate() override;

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });
};

