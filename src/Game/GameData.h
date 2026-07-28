#pragma once

#include "Timer.h"
#include <string>
#include "PlayerData.h"

enum class Weather : uint8_t { Clear, Rain, Storm };
enum class TimeOfDay : uint8_t { Day, Night };
enum class Season : uint8_t { Spring, Summer, Autumn, Winter };

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

	Weather GetWeather();
	TimeOfDay GetTimeOfDay();
	Season GetSeason();

	std::string GetMonthName();
	std::string GetDayName();

	float GetTimeScale();
	void SetTimeScale(float newScale);

	void Sleep();

	PlayerData& GetPlayerData() { return m_playerData; }

	void SetInventory(PlayerInventory* inv);

	void UnlockRecipe(const std::string& recipeID);
	bool IsRecipeUnlocked(const std::string& recipeID);

	const std::vector<std::string>& GetUnlockedRecipes();
};