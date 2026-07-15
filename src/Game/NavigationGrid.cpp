#include "pch.h"
#include "NavigationGrid.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentLib/LayerTiled.h"

void NavigationGrid::Initialize()
{
	MapTiledManager* mm = MapTiledManager::Get();

	MapTiled* map = mm->GetCurrentMap();

	std::vector<TileInfo*> tilesInfos;

	for (int i = 0; i < map->GetDimension().GetX() / 16; i++)
	{
		for (int j = 0; j < map->GetDimension().GetY() / 16; j++)
		{
			tilesInfos.push_back(map->GetTileAtGrid({i, j}).tileTiled);
		}
	}

	for (const TileInfo& info : canGrowTiles)
	{
		Vector2f center = {
			info.worldPos.GetX() + tileDim.GetX() / 2.f,
			info.worldPos.GetY() + tileDim.GetY() / 2.f
		};
	}
}
