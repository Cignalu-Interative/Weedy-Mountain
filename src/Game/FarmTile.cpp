#include "pch.h"
#include "FarmTile.h"
#include "FarmManager.h"
#include "SpriteContainer.h"
#include <iostream>
#include <cmath>

FarmTile::FarmTile()
{

}

void FarmTile::UpdateVisuals()
{
    RemoveSprite("soil");
    RemoveSprite("crop");

    switch (m_state)
    {
    case FarmTileState::IDLE:
        break;

    case FarmTileState::TILLED:
    case FarmTileState::WATERED:
        // Bêché sans graine
        AddSprite("soil", "soil_hole", GetDimension(), 1, { 0.f, 0.f }, GetIsGUI());
        break;

    case FarmTileState::PLANTED:
        // Graine plantée, pas arrosé
        AddSprite("soil", "soil_tilled", GetDimension(), 1, { 0.f,0.f }, GetIsGUI());
        break;

    case FarmTileState::GROWING:
    case FarmTileState::READY:
        // Graine plantée + arrosé
        AddSprite("soil", "soil_watered", GetDimension(), 1, { 0.f,0.f }, GetIsGUI());
        break;
    }

    if (m_crop.has_value() && m_growthStage >= 1)
    {
        int spriteIndex = std::min(m_growthStage - 1, (int)m_crop->m_growthSpritePath.size() - 1);
        AddSprite("crop", m_crop->m_growthSpritePath[spriteIndex], GetDimension(), 2, { 0.f,-5.f }, GetIsGUI());
    }
}

int FarmTile::ComputeGrowthStage() const
{
    if (m_crop.has_value() == false) 
        return 0;

    int totalStages = 5;
    float ratio = static_cast<float>(m_daysGrown) / m_crop->m_daysToGrow;
    return std::min(static_cast<int>(ratio * totalStages), totalStages - 1);
}

void FarmTile::Till(int p_currentDay)
{
    // Dé-bêcher si déjà bêché et vide
    if (m_state == FarmTileState::TILLED || m_state == FarmTileState::WATERED)
    {
        m_state = FarmTileState::IDLE;
        m_dayTilled = -1;
        m_wateredToday = false;
        UpdateVisuals();
        std::cout << "[FarmTile] De-beche" << std::endl;
        return;
    }

    if (m_state == FarmTileState::IDLE)
    {
        m_state = FarmTileState::TILLED;
        m_dayTilled = p_currentDay;
        m_wateredToday = false;
        UpdateVisuals();
        std::cout << "[FarmTile] Beche" << std::endl;
    }
}

void FarmTile::Water(int p_currentDay)
{
    if (m_state == FarmTileState::IDLE) 
        return;

    m_wateredToday = true;
    m_dayLastWatered = p_currentDay;
    m_daysWithoutWater = 0;

    if (m_state == FarmTileState::TILLED)
        m_state = FarmTileState::WATERED;

    else if (m_state == FarmTileState::PLANTED || m_state == FarmTileState::GROWING)
        m_state = FarmTileState::GROWING;

    UpdateVisuals();
    std::cout << "[FarmTile] Arrose" << std::endl;
}

void FarmTile::Plant(const CropDefinition& p_crop, int p_currentDay)
{
    if (m_state != FarmTileState::TILLED && m_state != FarmTileState::WATERED)
    {
        std::cout << "[FarmTile] Impossible de planter : sol non beche" << std::endl;
        return;
    }

    m_crop = p_crop;
    m_dayPlanted = p_currentDay;
    m_growthStage = 0;
    m_state = m_wateredToday ? FarmTileState::GROWING : FarmTileState::PLANTED;

    UpdateVisuals();
    std::cout << "[FarmTile] Plante : " << p_crop.m_name << std::endl;
}

void FarmTile::Harvest(int p_currentDay)
{
    if (m_state != FarmTileState::READY) 
        return;

    std::cout << "[FarmTile] Recolte : " << m_crop->m_name << " x" << m_crop->m_dropCount << " | XP : " << m_crop->m_xpOnHarvest << std::endl;

    m_crop.reset();
    m_growthStage = 0;
    m_dayPlanted = 0;
    m_daysGrown = 0;
    m_daysWithoutWater = 0;
    m_wateredToday = false;
    m_state = FarmTileState::TILLED;
    UpdateVisuals(); 
}

void FarmTile::OnNewDay(int p_currentDay)
{
    // Sol bêché vide - reset après 2 jours sans rien 
    if ((m_state == FarmTileState::TILLED || m_state == FarmTileState::WATERED) && m_crop.has_value() == false)
    {
        int daysSinceTill = p_currentDay - m_dayTilled;
        if (daysSinceTill >= 2)
        {
            m_state = FarmTileState::IDLE;
            m_dayTilled = -1;
            UpdateVisuals();
            std::cout << "[FarmTile] Sol retourné à l'état normal" << std::endl;
            return;
        }
    }

    if (m_crop.has_value() == false)
    {
        if (m_state == FarmTileState::WATERED)
        {
            m_state = FarmTileState::TILLED;
            UpdateVisuals();
        }
        m_wateredToday = false;
        return;
    }

    if (m_wateredToday)
    {
        m_daysGrown++;
        m_daysWithoutWater = 0;
    }
    else
    {
        m_daysWithoutWater++;
    }

    m_wateredToday = false;

    // Mort par manque d'eau 
    int threshold = m_crop->m_daysWithoutWaterBeforeDeath;
    int maxDays = threshold * 2;

    if (m_daysWithoutWater >= threshold)
    {
        if (m_daysWithoutWater >= maxDays)
        {
            std::cout << "[FarmTile] Plante morte (manque d'eau)" << std::endl;
            m_crop.reset();
            m_state = FarmTileState::TILLED;
            m_growthStage = 0;
            m_daysGrown = 0;
            m_daysWithoutWater = 0;
            UpdateVisuals();
            return;
        }

        float t = static_cast<float>(m_daysWithoutWater - threshold) / static_cast<float>(maxDays - threshold);
        float deathChance = 1.f - std::pow(1.f - t, 2.f);
        float roll = static_cast<float>(rand()) / RAND_MAX;

        if (roll < deathChance)
        {
            std::cout << "[FarmTile] Plante morte (chance=" << static_cast<int>(deathChance * 100) << "%)" << std::endl;
            m_crop.reset();
            m_state = FarmTileState::TILLED;
            m_growthStage = 0;
            m_daysGrown = 0;
            m_daysWithoutWater = 0;
            UpdateVisuals();
            return;
        }
    }

    // Progression
    m_growthStage = ComputeGrowthStage();

    if (m_daysGrown >= m_crop->m_daysToGrow)
    {
        m_state = FarmTileState::READY;
        std::cout << "[FarmTile] " << m_crop->m_name << " prêt !" << std::endl;
    }
    else
    {
        m_state = FarmTileState::PLANTED;
    }

    UpdateVisuals();
}