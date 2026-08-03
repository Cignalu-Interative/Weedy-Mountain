#include "pch.h"
#include "NavigationGrid.h"


void NavigationGrid::Initialize()
{
	MapTiledManager* mm = MapTiledManager::Get();

	MapTiled* map = mm->GetCurrentMap();
	m_gridDimension = map->GetDimension() / 16;

	std::vector<TileInfo> tilesInfos;

	for (int i = 0; i < m_gridDimension.GetX(); i++)
	{
		for (int j = 0; j < m_gridDimension.GetY(); j++)
		{
			tilesInfos.push_back(map->GetTileAtGrid({i, j}));
		}
	}

	for (const TileInfo &info : tilesInfos)
	{
		m_mapTiles.push_back(InitializeTile(info));
	}
}

NavTile* NavigationGrid::InitializeTile(TileInfo p_infos)
{
	NavTile* tile = new NavTile;

	tile->pos = p_infos.worldPos;
	tile->gridPos = p_infos.gridPos;

	if (p_infos.IsValid() == false)
	{
		static int a = 0;
		std::cout << a++ << std::endl;
		return nullptr;
	}

	if (p_infos.layer->GetLayerName() == "Grass")
	{
		tile->type = TileType::GRASS;
		tile->canBeCrossed = false;
	}
	else if (p_infos.layer->GetLayerName() == "Water")
	{
		tile->type = TileType::WATER;
		tile->canBeCrossed = false;
	}
	else if (p_infos.layer->GetLayerName() == "NearHouses" ||
		p_infos.layer->GetLayerName() == "Buidings")
	{
		tile->type = TileType::STRUCTURE;
		tile->canBeCrossed = false;
	}
	else if (p_infos.layer->GetLayerName() == "Path" ||
		p_infos.layer->GetLayerName() == "Floor" ||
		p_infos.layer->GetLayerName() == "Floor 2")
	{
		tile->type = TileType::PATH;
		tile->canBeCrossed = true;
	}
	else
	{
		tile->type = TileType::UNKNOWN;
		tile->canBeCrossed = false;
	}

	return tile;
}

void NavigationGrid::SetTileCrossedByType(TileType p_type, bool p_value)
{
	for (NavTile* tile : m_mapTiles)
	{
		if (tile == nullptr)
		{
			continue;
		}

		if (tile->type == p_type)
		{
			tile->canBeCrossed = p_value;
		}
	}
}

NavTile* NavigationGrid::GetTileByGridPos(Vector2i p_gridPos)
{
	return m_mapTiles[p_gridPos.GetX() * m_gridDimension.GetY() + p_gridPos.GetY()];
}

std::vector<NavTile*> NavigationGrid::GetTilesByType(TileType p_type)
{
	std::vector<NavTile*> tiles;

	for (NavTile* tile : m_mapTiles)
	{
		if (tile == nullptr)
		{
			continue;
		}

		if (tile->type == p_type)
		{
			tiles.push_back(tile);
		}
	}

	return tiles;
}
