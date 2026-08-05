#pragma once
#include "2DPendentEngine/Entity.h"
#include "GameData.h"


class NightEffect : public Entity
{
private:

	int m_sunrise;
	int m_endSunrise;
	int m_sunset;
	int m_endSunset;

	Season m_season;

	void OnInitialize() override;
	void OnUpdate() override;
};

