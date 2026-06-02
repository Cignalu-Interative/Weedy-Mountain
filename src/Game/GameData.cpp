#include "pch.h"
#include "GameData.h"

//Trucs qu'on peut changer si on veut changer la durée plus tard

constexpr float IRL_SECONDS_PER_GAME_HOUR = 0.2;

constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY = 5;
constexpr int DAYS_PER_WEEK = 7;
constexpr int DAYS_PER_MONTH = 10;
constexpr int MONTHS_PER_YEAR = 4;



void GameData::Initialize()
{
	m_timer.SetCurrentTime();
}

void GameData::Update()
{
	m_timeSpent += m_timer.GetElapsed();
}



float GameData::GetTimeSpent()
{
	return m_timeSpent;
}



int GameData::GetYear()
{
	float secPerYear = MONTHS_PER_YEAR * DAYS_PER_MONTH * HOURS_PER_DAY * IRL_SECONDS_PER_GAME_HOUR;

	return m_timeSpent / secPerYear + 1;
}

int GameData::GetMonth()
{
	float secPerMonth = DAYS_PER_MONTH * HOURS_PER_DAY * IRL_SECONDS_PER_GAME_HOUR;

	int totalMonth = m_timeSpent / secPerMonth;

	return totalMonth % MONTHS_PER_YEAR + 1;
}

int GameData::GetDay()
{
	float secPerDay = HOURS_PER_DAY * IRL_SECONDS_PER_GAME_HOUR;

	int totalDay = m_timeSpent / secPerDay;

	return totalDay % DAYS_PER_MONTH + 1;
}



int GameData::GetHour()
{
	int totalH = m_timeSpent / IRL_SECONDS_PER_GAME_HOUR;
	return totalH % HOURS_PER_DAY;
}

int GameData::GetMinute()
{
	int time = m_timeSpent / IRL_SECONDS_PER_GAME_HOUR;
	return ((m_timeSpent - time * IRL_SECONDS_PER_GAME_HOUR) * MINUTES_PER_HOUR) / IRL_SECONDS_PER_GAME_HOUR; //regardder si on peut faire mieux
}



std::string GameData::GetMonthName()
{
	static const char* seasons[MONTHS_PER_YEAR] =
	{
		"Printemps",
		"Ete",
		"Automne",
		"Hiver"
	};

	return seasons[GetMonth() - 1];
}

std::string GameData::GetDayName()
{
	static const char* days[DAYS_PER_WEEK] =
	{
		"Lundi",
		"Mardi",
		"Mercredi",
		"Jeudi",
		"Vendredi",
		"Samedi",
		"Dimanche"
	};

	float secPerDay = HOURS_PER_DAY * IRL_SECONDS_PER_GAME_HOUR;

	int totalDay = m_timeSpent / secPerDay;

	return days[totalDay % DAYS_PER_WEEK];
}