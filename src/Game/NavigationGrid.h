#pragma once
#include "2DPendentLib/Utils.h"

enum class TileType
{
	GRASS,
	WATER,
	PATH,


	COUNT
};


class NavigationGrid
{
private:

	struct Tile
	{
		Vector2f pos;
		TileType type;
		bool canBeCrossed;
	};

	std::vector<Tile> m_mapTiles;
};

