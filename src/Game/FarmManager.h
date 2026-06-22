#pragma once
#include <vector>
#include "FarmTile.h"

class Render;
class MapTiled;

class FarmManager
{
private:
    std::vector<FarmTile*> m_farmTiles;
    int m_lastDay = -1;
    int m_lastAgriDay = -1;

    int ComputeAgriDay() const;

    FarmManager() = default;

public:
    static FarmManager* Get();

    // Appelé une fois au démarrage de la scène
    void Initialize(MapTiled* p_map, const std::string& p_layerName = "");

    // Enregistre une FarmTile (pour les tiles créées manuellement)
    void RegisterTile(FarmTile* p_tile);

    // Appelé chaque frame — déclenche OnNewDay si le jour a changé
    void Update();

    // Trouve la FarmTile la plus proche d'une position monde
    FarmTile* GetNearestTile(Vector2f p_worldPos, float p_maxDistance = 9999.f) const;

    // Trouve la FarmTile en collision avec un collider donné
    FarmTile* GetTileInActionBox(Collider* p_actionBox) const;

    std::vector<FarmTile*> GetAllFarmTiles() { return m_farmTiles; }

    void Clear();
};