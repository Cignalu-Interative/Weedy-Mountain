#pragma once
#include <vector>
#include "FarmTile.h"
#include "CaveTile.h"
#include "Tile.h"

enum class EnviroSpawnName
{
	Tree,
	Shrub,
	Grass,
	Rock,
	Stone,

	IronOre,
	GoldOre,
	DiamondOre,

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
	std::vector<EnviroInfo*> m_farmEnviroList;
	std::vector<EnviroInfo*> m_caveEnviroList;
	std::vector<Tile> m_freeTiles;
	int m_farmDayCounter = 0;
	int m_caveDayCounter = 0;

	int m_lastDay = -1;
protected:

	void CreateEnviroProps(EnviroSpawnName _name, int _tile, Scene* _scene);

	void SpawnEnviroProps();

	int GetEnviroDay();

	void NewGameEnviroSpawn();

	void UpdateFreeTiles();

	std::vector<Tile> GetFreeTiles(bool isCave) const;

public:

	void InitEnviroSpawn();

	static EnviroSpawn* Get();

	void Update();

};
