#pragma once
#include "CaveTile.h"

class CaveManager
{
	std::vector<CaveTile*> m_caveTiles;
public:
	static CaveManager* Get();

	void Initialize(MapTiled* p_map, const std::string& p_layerName);

	void RegisterTile(CaveTile* p_tile);

	std::vector<CaveTile*> GetAllCaveTiles() { return m_caveTiles; }
	
	void Clear();
};

