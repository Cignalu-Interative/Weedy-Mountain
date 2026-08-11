#include "pch.h"
#include "WeatherManager.h"

WeatherManager& WeatherManager::Get()
{
	static WeatherManager Instance;
	return Instance;
}

void WeatherManager::Initialize()
{
	int m_currentDay = 0;
	int m_prevDay = 0;
}

void WeatherManager::Update()
{
	GameData gd = GameData::Get();
	m_currentDay = gd.GetDay();

	Season currentSeason = gd.GetSeason();

	if (m_prevDay != m_currentDay)
	{
		m_prevDay = m_currentDay;
		int proba = Aleatory::GetInt(1, 100);

		switch (currentSeason)
		{
		case Season::Spring:

			if (proba <= 35)
			{
				m_currentWeather = Weather::Rain;
			}
			else if (proba > 35 && proba <= 40)
			{
				m_currentWeather = Weather::Storm;
			}
			else
			{
				m_currentWeather = Weather::Clear;
			}
			break;

		case Season::Summer:

			if (proba <= 20)
			{
				m_currentWeather = Weather::Rain;
			}
			else if (proba > 20 && proba <= 32)
			{
				m_currentWeather = Weather::Storm;
			}
			else
			{
				m_currentWeather = Weather::Clear;
			}
			break;

		case Season::Autumn:

			if (proba <= 40)
			{
				m_currentWeather = Weather::Rain;
			}
			else if (proba > 40 && proba <= 48)
			{
				m_currentWeather = Weather::Storm;
			}
			else
			{
				m_currentWeather = Weather::Clear;
			}
			break;

		case Season::Winter:

			if (proba <= 25)
			{
				m_currentWeather = Weather::Rain;
			}
			else if (proba > 25 && proba <= 21)
			{
				m_currentWeather = Weather::Storm;
			}
			else
			{
				m_currentWeather = Weather::Clear;
			}
			break;
		}
	}
}

