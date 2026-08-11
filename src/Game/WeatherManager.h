#pragma once
#include "GameData.h"

class WeatherManager
{
private :

	Weather m_currentWeather = Weather::Clear;
	int m_currentDay;
	int m_prevDay;

public :

	static WeatherManager& Get();

	void Initialize();
	void Update();
	Weather GetWeather() { return m_currentWeather; }
};

