#pragma once

#include "Timer.h"
#include <string>
#include "PlayerData.h"
#include "PlayerCollection.h"

#include "Savable.h"

enum class Weather : uint8_t { Clear, Rain, Storm };
enum class TimeOfDay : uint8_t { Day, Night };
enum class Season : uint8_t { Spring, Summer, Autumn, Winter };

enum class CurrentScene
{
	FARM,
	VILLAGE,
	CAVE
};

class GameData : public Savable
{
private:

	Timer m_timer;

	float m_timeSpent = 0.0f;

	float m_timeScale = 1.0f;

	std::vector<std::string> m_unlockedRecipes;

	GameData();

	PlayerData m_playerData;
	PlayerCollection m_playerCollection;

	CurrentScene m_currentScene = CurrentScene::FARM;

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
	PlayerCollection& GetPlayerCollection() { return m_playerCollection; }

	PlayerJob* GetJobByName(JobType p_type)
	{
		return m_playerData.GetJobByName(p_type);
	}

	void SetInventory(PlayerInventory* inv);

	void UnlockRecipe(const std::string& recipeID);
	bool IsRecipeUnlocked(const std::string& recipeID);

	const std::vector<std::string>& GetUnlockedRecipes();

	void SetCurrentScene(CurrentScene p_currentScene) { m_currentScene = p_currentScene; }
	const CurrentScene GetCurrentScene() const { return m_currentScene; }

	static std::string SceneIDFromCurrentScene(CurrentScene p_scene);
	static CurrentScene CurrentSceneFromSceneID(const std::string& p_id);

	std::string GetSaveID() const override { return "gameData"; }
	void Save(nlohmann::json& p_out) override;
	void Load(const nlohmann::json& p_in) override;
};