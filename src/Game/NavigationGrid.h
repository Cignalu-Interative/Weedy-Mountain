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

struct Tile
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

	std::vector<Tile*> m_mapTiles;

	Tile* InitializeTile(TileInfo p_infos);

public:

	NavigationGrid();

	std::vector<Tile*> GetMapTiles() { return m_mapTiles; }

	void SetTileCrossedByType(TileType p_type, bool p_value);

	Tile* GetTileByGridPos(Vector2i p_gridPos);
	std::vector<Tile*> GetTilesByType(TileType p_type);

	Vector2i GetGridDimension() { return m_gridDimension; }

};

