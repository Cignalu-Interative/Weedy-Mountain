#pragma once
#include "2DPendentEngine/Entity.h"
#include "CropDefinition.h"
#include <optional>

enum class FarmTileState
{
    IDLE,       // sol normal, rien
    TILLED,     // bêché
    WATERED,    // bêché + arrosé 
    PLANTED,    // planté mais pas encore arrosé 
    GROWING,    // planté + arrosé (pousse en cours)
    READY       // prêt à récolter
};

class FarmTile : public Entity
{
private:

    FarmTileState m_state = FarmTileState::IDLE;

    // Plante courante (nullopt si rien de planté)
    std::optional<CropDefinition> m_crop;

    int m_dayPlanted = 0;  
    int m_growthStage = 0; 
    int m_dayLastWatered = -1; 
    int m_daysWithoutWater = 0;  
    int m_dayTilled = -1; 

    bool m_wateredToday = false;

    void UpdateVisuals();
    int ComputeGrowthStage(int p_currentDay) const;

public:

    FarmTile();

    void OnInitialize() override {}

    // Actions joueur
    void Till(int p_currentDay); // bêcher / dé-bêcher
    void Water(int p_currentDay); // arroser
    void Plant(const CropDefinition& p_crop, int p_currentDay); // planter
    void Harvest(int p_currentDay); // récolter

    // Appelé par FarmManager à chaque nouveau jour
    void OnNewDay(int p_currentDay);

    FarmTileState GetState() const { return m_state; }
    bool IsWatered() const { return m_wateredToday; }
    bool IsReady() const { return m_state == FarmTileState::READY; }
    bool HasCrop() const { return m_crop.has_value(); }
    const CropDefinition* GetCrop() const { return m_crop.has_value() ? &m_crop.value() : nullptr; }
};