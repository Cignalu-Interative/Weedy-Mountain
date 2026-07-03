#pragma once
#include <vector>

enum class EnviroSpawnName
{
	Tree,
	Shrub,
	Grass,
	Rock,
	Stone,

	Count
};

struct EnviroSpawnStats {
	int m_minSpawn = 0;
	int m_maxSpawn = 1;
};

struct EnviroInfo {
	EnviroSpawnName m_name;
	EnviroSpawnStats m_stats;
};

class EnviroSpawn
{
	int m_maxTiles = 100;
	int m_numOfEnviro = 0;
	std::vector<EnviroInfo*> m_enviroList;

	int m_lastDay = -1;
protected:

	void CreateEnviroProps(EnviroSpawnName _name, int _tile);

	void SpawnEnviroProps();

	int GetEnviroDay();

	void NewGameEnviroSpawn();

public:

	void InitEnviroSpawn();

	static EnviroSpawn* Get();

	void Update();

};

