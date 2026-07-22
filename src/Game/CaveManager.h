#pragma once
#include "CaveTile.h"
class CaveManager
{
	std::vector<CaveTile*> m_caveTiles;
public:
	void Initialize(MapTiled* p_map, const std::string& p_layerName);
	
	void Clear();
};

