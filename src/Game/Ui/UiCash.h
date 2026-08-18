#pragma once
#include "Ui.h"

class UiCash : public Ui
{
	void OnInitialize() override;
	void OnUpdate() override;

public:
	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });

	void SetIsVisible(bool p_isVisible);
};

