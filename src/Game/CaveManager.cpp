#include "pch.h"
#include "CaveManager.h"
#include "2DPendentLib/MapTiled.h"
#include "CollisionManager.h"
#include "GameData.h"

CaveManager* CaveManager::Get() {
    static CaveManager instance;
    return &instance;
}

void CaveManager::Initialize(MapTiled* p_map, const std::string& p_layerName)
{
    if (!p_map)
        return;

    Clear();

    auto canGrowTiles = p_map->GetAllTilesWithProperty("CanGrow", p_layerName);
    Vector2i tileDim = p_map->GetTileDimension();

    for (const TileInfo& info : canGrowTiles)
    {
        Vector2f center = {
            info.worldPos.GetX() + tileDim.GetX() / 2.f,
            info.worldPos.GetY() + tileDim.GetY() / 2.f
        };

        CaveTile* tile = new CaveTile();
        tile->SetPosition(center);
        tile->SetDimension(tileDim);

        m_caveTiles.push_back(tile);
    }

    std::cout << "[CaveManager] " << m_caveTiles.size() << " CaveTiles initialisées" << std::endl;
}

void CaveManager::RegisterTile(CaveTile* p_tile)
{
    if (p_tile)
        m_caveTiles.push_back(p_tile);
}

void CaveManager::Clear() {
	m_caveTiles.clear();
}

