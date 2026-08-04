#include "pch.h"
#include "FarmManager.h"
#include "2DPendentLib/MapTiled.h"
#include "CollisionManager.h"
#include "GameData.h"
#include <algorithm>

#include "SaveManager.h"

FarmManager* FarmManager::Get()
{
    static FarmManager instance;
    return &instance;
}

void FarmManager::Initialize(MapTiled* p_map, const std::string& p_layerName)
{
    if (!p_map) 
        return;

    Clear();

    auto canGrowTiles = p_map->GetAllTilesWithProperty("canGrow", p_layerName);
    Vector2i tileDim = p_map->GetTileDimension();

    for (const TileInfo& info : canGrowTiles)
    {
        Vector2f center = {
            info.worldPos.GetX() + tileDim.GetX() / 2.f,
            info.worldPos.GetY() + tileDim.GetY() / 2.f
        };

        FarmTile* tile = new FarmTile();
        tile->SetPosition(center);
        tile->SetDimension(tileDim);

        m_farmTiles.push_back(tile);
    }

    std::cout << "[FarmManager] " << m_farmTiles.size() << " FarmTiles initialisées" << std::endl;
}

void FarmManager::RegisterTile(FarmTile* p_tile)
{
    if (p_tile)
        m_farmTiles.push_back(p_tile);
}

int FarmManager::ComputeAgriDay() const
{
    constexpr float IRL_SECONDS_PER_GAME_HOUR = 40.0f;
    constexpr float HOURS_PER_DAY = 24.f;
    constexpr float SIX_HOURS = 6.f * IRL_SECONDS_PER_GAME_HOUR;

    float secPerDay = HOURS_PER_DAY * IRL_SECONDS_PER_GAME_HOUR;
    float timeSpent = GameData::Get().GetTimeSpent();

    float shifted = timeSpent - SIX_HOURS;

    if (shifted < 0.f)
        shifted = 0.f;

    return static_cast<int>(shifted / secPerDay);
}

FarmManager::FarmManager()
{
    SaveManager::Get().RegisterProvider(this);
}

void FarmManager::Update()
{
    int agriDay = ComputeAgriDay();

    if (agriDay == m_lastAgriDay) return;

    // Nouveau jour agricole détecté (à 6h)
    if (m_lastAgriDay != -1)
    {
        int currentDay = GameData::Get().GetDay();

        for (FarmTile* tile : m_farmTiles)
            tile->OnNewDay(currentDay);

        std::cout << "[FarmManager] Nouveau jour agricole : " << currentDay << std::endl;
    }

    m_lastAgriDay = agriDay;
}

FarmTile* FarmManager::GetNearestTile(Vector2f p_worldPos, float p_maxDistance) const
{
    FarmTile* nearest = nullptr;
    float bestDist = p_maxDistance * p_maxDistance;

    for (FarmTile* tile : m_farmTiles)
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

FarmTile* FarmManager::GetTileInActionBox(Collider* p_actionBox) const
{
    if (!p_actionBox)
        return nullptr;

    FarmTile* best = nullptr;
    float bestOverlap = 0.f;

    for (FarmTile* tile : m_farmTiles)
    {
        Collider* tileCol = tile->GetCollider();

        if (!tileCol) 
            continue;

        // AABB overlap
        Vector2f posA = p_actionBox->GetPosition({ 0.f, 0.f });
        Vector2i dimA = p_actionBox->GetDimension();
        Vector2f posB = tileCol->GetPosition({ 0.f, 0.f });
        Vector2i dimB = tileCol->GetDimension();

        float overlapX = std::min(posA.GetX() + dimA.GetX(), posB.GetX() + dimB.GetX()) - std::max(posA.GetX(), posB.GetX());
        float overlapY = std::min(posA.GetY() + dimA.GetY(), posB.GetY() + dimB.GetY()) - std::max(posA.GetY(), posB.GetY());

        if (overlapX <= 0.f || overlapY <= 0.f) 
            continue;

        float overlap = overlapX * overlapY;
        if (overlap > bestOverlap)
        {
            bestOverlap = overlap;
            best = tile;
        }
    }
    return best;
}

void FarmManager::Clear()
{
    m_farmTiles.clear();
}

void FarmManager::Save(nlohmann::json& out)
{
    json arr = json::array();

    for (FarmTile* tile : m_farmTiles)
    {
        json t;
        tile->Save(t);
        arr.push_back(t);
    }

    out["tiles"] = arr;
}

void FarmManager::Load(const nlohmann::json& in) 
{
    for (auto& t : in.value("tiles", json::array()))
    {
        float x = t.value("posX", 0.f);
        float y = t.value("posY", 0.f);

        FarmTile* target = nullptr;

        for (FarmTile* tile : m_farmTiles)
        {
            Vector2f p = tile->GetPosition();

            if (std::abs(p.GetX() - x) < 0.1f && std::abs(p.GetY() - y) < 0.1f)
            {
                target = tile;
                break;
            }
        }

        if (target) 
            target->Load(t);
    }
}
