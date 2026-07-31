#include "pch.h"
#include "GameData.h"
#include "RecipeDataBase.h"
#include "InventorySlot.h"
#include "UiChest.h"

//Trucs qu'on peut changer si on veut changer la dur�e plus tard

constexpr float IRL_SECONDS_PER_GAME_HOUR = 40.0;

constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY = 24;
constexpr int DAYS_PER_WEEK = 7;
constexpr int DAYS_PER_MONTH = 30;
constexpr int MONTHS_PER_YEAR = 4;

GameData& GameData::Get()
{
	static GameData Instance;
	return Instance;
}

void GameData::Initialize()
{
	m_timer.SetCurrentTime();

}

void GameData::Update()
{
	m_timeSpent += m_timer.GetElapsed() * m_timeScale;
	//m_playerData.m_playerInventory.OnUpdate();
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

Weather GameData::GetWeather()
{
	return Weather::Clear;
}

TimeOfDay GameData::GetTimeOfDay()
{
	if (GetHour() < 5 || GetHour() > 20)
		return TimeOfDay::Night;

	return TimeOfDay::Day;
}

Season GameData::GetSeason()
{
	return (Season)(GetMonth() - 1);
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



float GameData::GetTimeScale()
{
	return m_timeScale;
}

void GameData::SetTimeScale(float newScale)
{
	m_timeScale = newScale;
}



void GameData::Sleep()
{
	float secPerDay = HOURS_PER_DAY * IRL_SECONDS_PER_GAME_HOUR;
	float sixAM = 6.0f * IRL_SECONDS_PER_GAME_HOUR;

	int totalDay = (m_timeSpent / secPerDay);
	float timeOfDay = m_timeSpent - (totalDay * secPerDay);

	m_timeSpent = totalDay * secPerDay + (6.0f * IRL_SECONDS_PER_GAME_HOUR);


	if (timeOfDay < sixAM) //si on est entre 0h et 5h59
	{
		m_timeSpent = totalDay * secPerDay + sixAM;
	}
	else //si on est apres 6h
	{
		m_timeSpent = (totalDay + 1) * secPerDay + sixAM;
	}
}

void GameData::SetInventory(PlayerInventory* inv)
{
	if (inv == nullptr)
		return;

	if (m_playerData.m_playerInventory == inv)
		return;

	m_playerData.m_playerInventory = inv;
}
void GameData::UnlockRecipe(const std::string& recipeID)
{
	if (IsRecipeUnlocked(recipeID) || RecipeDataBase::Get()->GetRecipe(recipeID) != nullptr)
		return;

	m_unlockedRecipes.push_back(recipeID);
}

bool GameData::IsRecipeUnlocked(const std::string& recipeID)
{
	return std::find(m_unlockedRecipes.begin(), m_unlockedRecipes.end(), recipeID) != m_unlockedRecipes.end();
}

const std::vector<std::string>& GameData::GetUnlockedRecipes()
{
	return m_unlockedRecipes;
}

