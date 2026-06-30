#pragma once
#include "Ui.h"

#include "UiLife.h"
#include "UiToxicity.h"
#include "UiStamina.h"


class UiStats : public Ui
{
private : 
	std::vector<Ui> m_statsUi;


public : 
	void OnInitialize() override;
	void OnUpdate() override;


};

