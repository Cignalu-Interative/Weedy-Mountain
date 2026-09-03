#pragma once
#include "Ui.h"

#include "UiLife.h"
#include "UiToxicity.h"
#include "UiStamina.h"


class UiStats : public Ui
{
private : 
	std::vector<Ui> m_statsUi;

	UiLife* m_uiLife = nullptr;
	UiToxicity* m_uiToxicity = nullptr;
	UiStamina* m_uiStamina = nullptr;

	void OnInitialize() override;
	void OnUpdate() override;

public:
	void SetVisible(bool p_value);
};

