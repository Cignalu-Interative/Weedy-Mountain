#include "pch.h"
#include "GameData.h"
#include "RecipeDataBase.h"
#include "InventorySlot.h"
#include "UiChest.h"
#include "RecipesMenu.h"
#include "PlayerData.h"

#include "SaveManager.h"

//Trucs qu'on peut changer si on veut changer la dur�e plus tard

constexpr float IRL_SECONDS_PER_GAME_HOUR = 40.0;

constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY = 24;
constexpr int DAYS_PER_WEEK = 7;
constexpr int DAYS_PER_MONTH = 30;
constexpr int MONTHS_PER_YEAR = 4;

GameData::GameData()
{
	SaveManager::Get().RegisterProvider(this);
}

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

Season GameData::GetSeason()
{
	return (Season)(GetMonth() - 1);
}

TimeOfDay GameData::GetTimeOfDay()
{
	if (GetSeason() == Season::Spring)
	{
		if (GetHour() > 20 && GetHour() < 7)
			return TimeOfDay::Night;
	}

	else if (GetSeason() == Season::Summer)
	{
		if (GetHour() > 21 && GetHour() < 6)
			return TimeOfDay::Night;
	}

	else if (GetSeason() == Season::Autumn)
	{
		if (GetHour() > 20 && GetHour() < 7)
			return TimeOfDay::Night;
	}

	else if (GetSeason() == Season::Winter)
	{
		if (GetHour() > 19 && GetHour() < 8)
			return TimeOfDay::Night;
	}
	return TimeOfDay::Day;
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
	if (IsRecipeUnlocked(recipeID) || RecipeDataBase::Get()->GetRecipe(recipeID) == nullptr)
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

void GameData::Save(nlohmann::json& out)
{
	out["timeSpent"] = m_timeSpent;
	out["timeScale"] = m_timeScale;
	out["unlockedRecipes"] = m_unlockedRecipes;

	auto& stats = m_playerData.m_stats;
	out["stats"] = {
		{"hp", stats.m_hp}, {"maxHp", stats.m_maxHp},
		{"stamina", stats.m_stamina}, {"maxStamina", stats.m_maxStamina},
		{"toxicity", stats.m_toxicity},
		{"harvestEfficiency", stats.m_harvestEffiency},
		{"farmerSpeed", stats.m_farmerSpeed},
		{"miningSpeed", stats.m_miningSpeed}
	};

	out["money"] = m_playerData.m_money;
	out["cash"] = m_playerData.m_cash;

	json jobs = json::array();

	for (PlayerJob* job : m_playerData.m_jobs)
		jobs.push_back({ {"name", (int)job->m_name}, {"lvl", job->m_lvl}, {"exp", job->m_exp}, {"expNeeded", job->m_expNeeded} });
	
	out["jobs"] = jobs;
}

void GameData::Load(const nlohmann::json& in)
{
	m_timeSpent = in.value("timeSpent", 0.0f);
	m_timeScale = in.value("timeScale", 1.0f);
	m_unlockedRecipes = in.value("unlockedRecipes", std::vector<std::string>{});

	if (in.contains("stats"))
	{
		auto& s = in["stats"];
		auto& stats = m_playerData.m_stats;
		stats.m_hp = s.value("hp", stats.m_maxHp);
		stats.m_maxHp = s.value("maxHp", stats.m_maxHp);
		stats.m_stamina = s.value("stamina", stats.m_maxStamina);
		stats.m_maxStamina = s.value("maxStamina", stats.m_maxStamina);
		stats.m_toxicity = s.value("toxicity", 0.f);
		stats.m_harvestEffiency = s.value("harvestEfficiency", 1.f);
		stats.m_farmerSpeed = s.value("farmerSpeed", 1.f);
		stats.m_miningSpeed = s.value("miningSpeed", 1.f);
	}

	m_playerData.m_money = in.value("money", 200000);
	m_playerData.m_cash = in.value("cash", 0);

	for (PlayerJob* job : m_playerData.m_jobs)
		delete job;

	m_playerData.m_jobs.clear();

	for (auto& j : in.value("jobs", json::array()))
	{
		PlayerJob* job = new PlayerJob();
		job->m_name = (JobType)j.value("name", 0);
		job->m_lvl = j.value("lvl", 1);
		job->m_exp = j.value("exp", 0.f);
		job->m_expNeeded = j.value("expNeeded", 100.f);
		m_playerData.m_jobs.push_back(job);
	}
}
