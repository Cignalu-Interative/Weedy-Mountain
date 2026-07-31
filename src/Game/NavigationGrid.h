#pragma once
#include "2DPendentLib/Utils.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentLib/LayerTiled.h"

enum class TileType
{
	GRASS,
	WATER,
	PATH,
	STRUCTURE,
	UNKNOWN,

	COUNT
};

struct NavTile
{
	Vector2f pos;
	Vector2i gridPos;
	TileType type;
	bool canBeCrossed;
};

class NavigationGrid
{
private:

	Vector2i m_gridDimension;

	std::vector<NavTile*> m_mapTiles;

	NavTile* InitializeTile(TileInfo p_infos);

public:

	NavigationGrid();

	std::vector<NavTile*> GetMapTiles() { return m_mapTiles; }

	void SetTileCrossedByType(TileType p_type, bool p_value);

	NavTile* GetTileByGridPos(Vector2i p_gridPos);
	std::vector<NavTile*> GetTilesByType(TileType p_type);

	Vector2i GetGridDimension() { return m_gridDimension; }

};

