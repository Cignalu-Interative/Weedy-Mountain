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

CaveTile* CaveManager::GetNearestTile(Vector2f p_worldPos, float p_maxDistance) const
{
    CaveTile* nearest = nullptr;
    float bestDist = p_maxDistance * p_maxDistance;

    for (CaveTile* tile : m_caveTiles)
    {
        Vector2f tileCenter = tile->GetPosition({ 0.5f, 0.5f });
        float dx = tileCenter.GetX() - p_worldPos.GetX();
        float dy = tileCenter.GetY() - p_worldPos.GetY();
        float distSq = dx * dx + dy * dy;

        if (distSq < bestDist)
        {
            bestDist = distSq;
            nearest = tile;
        }
    }
    return nearest;
}

void CaveManager::Clear() {
	m_caveTiles.clear();
}

