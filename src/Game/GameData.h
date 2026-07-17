#pragma once

#include "Timer.h"
#include <string>
#include "PlayerData.h"

class GameData
{
private:

	Timer m_timer;

	float m_timeSpent = 0.0f;

	float m_timeScale = 1.0f;

	std::vector<std::string> m_unlockedRecipes;

	GameData() = default;

	PlayerData m_playerData;

public:

	static GameData& Get();

	void Initialize();
	void Update();

	float GetTimeSpent();

	int GetYear();
	int GetMonth();
	int GetDay();

	int GetHour();
	int GetMinute();

	std::string GetMonthName();
	std::string GetDayName();

	float GetTimeScale();
	void SetTimeScale(float newScale);

	void Sleep();

	PlayerData& GetPlayerData() { return m_playerData; }

	PlayerJob* GetJobByName(JobType p_type)
	{
		for (PlayerJob* job : m_playerData.m_jobs)
		{
			if (job->m_name == p_type)
				return job;
		}
	}

	void SetInventory(PlayerInventory* inv);
	void UnlockRecipe(const std::string& recipeID);
	bool IsRecipeUnlocked(const std::string& recipeID);

	const std::vector<std::string>& GetUnlockedRecipes();
};