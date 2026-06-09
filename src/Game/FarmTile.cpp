#include "pch.h"
#include "FarmTile.h"
#include "FarmManager.h"
#include <iostream>
#include <cmath>

void FarmTile::UpdateVisuals()
{
    RemoveSprite("soil");
    RemoveSprite("soil_w");
    RemoveSprite("crop");

    switch (m_state)
    {
    case FarmTileState::IDLE:
        break;

    case FarmTileState::TILLED:
        AddSprite("soil", "soil_tilled", GetDimension(), 0, { 0.f,0.f }, GetIsGUI());
        break;

    case FarmTileState::WATERED:
    case FarmTileState::GROWING:
    case FarmTileState::READY:
        AddSprite("soil_w", "soil_watered", GetDimension(), 0, { 0.f,0.f }, GetIsGUI());
        break;

    case FarmTileState::PLANTED:
        AddSprite("soil", "soil_tilled", GetDimension(), 0, { 0.f,0.f }, GetIsGUI());
        break;
    }

    if (m_crop.has_value() && m_state != FarmTileState::TILLED && m_state != FarmTileState::WATERED && m_state != FarmTileState::IDLE)
    {
        int stage = std::min(m_growthStage, (int)m_crop->m_growthSpritePath.size() - 1);
        const std::string& cropTex = m_crop->m_growthSpritePath[stage];
        AddSprite("crop", cropTex, GetDimension(), 1, { 0.f,0.f }, GetIsGUI());
    }
}

int FarmTile::ComputeGrowthStage(int p_currentDay) const
{
    if (m_crop.has_value() == false) 
        return 0;

    int daysGrown = p_currentDay - m_dayPlanted;
    int stages = static_cast<int>(m_crop->m_growthSpritePath.size());
    float ratio = static_cast<float>(daysGrown) / m_crop->m_daysToGrow;

    return std::min(static_cast<int>(ratio * stages), stages - 1);
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
        std::cout << "[FarmTile] Dé-bêché" << std::endl;
        return;
    }

    if (m_state == FarmTileState::IDLE)
    {
        m_state = FarmTileState::TILLED;
        m_dayTilled = p_currentDay;
        m_wateredToday = false;
        UpdateVisuals();
        std::cout << "[FarmTile] Bêché" << std::endl;
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
    std::cout << "[FarmTile] Arrosé" << std::endl;
}

void FarmTile::Plant(const CropDefinition& p_crop, int p_currentDay)
{
    if (m_state != FarmTileState::TILLED && m_state != FarmTileState::WATERED)
    {
        std::cout << "[FarmTile] Impossible de planter : sol non bêché" << std::endl;
        return;
    }

    m_crop = p_crop;
    m_dayPlanted = p_currentDay;
    m_growthStage = 0;
    m_state = m_wateredToday ? FarmTileState::GROWING : FarmTileState::PLANTED;

    UpdateVisuals();
    std::cout << "[FarmTile] Planté : " << p_crop.m_name << std::endl;
}

void FarmTile::Harvest(int p_currentDay)
{
    if (m_state != FarmTileState::READY)
    {
        std::cout << "[FarmTile] Pas prêt à récolter" << std::endl;
        return;
    }

    int drop = m_crop->m_minDrop + rand() % (m_crop->m_maxDrop - m_crop->m_minDrop + 1);

    std::cout << "[FarmTile] Récolte : " << m_crop->m_name << " x" << drop << " | XP : " << m_crop->m_xpOnHarvest << std::endl;

    m_crop.reset();
    m_growthStage = 0;
    m_dayPlanted = 0;
    m_daysWithoutWater = 0;
    m_wateredToday = false;
    m_state = FarmTileState::TILLED;
}

void FarmTile::OnNewDay(int p_currentDay)
{
    m_wateredToday = false;

    // ── Sol bêché vide — reset après 3 jours sans rien ───────────────
    if ((m_state == FarmTileState::TILLED || m_state == FarmTileState::WATERED) && m_crop.has_value() == false)
    {
        int daysSinceTill = p_currentDay - m_dayTilled;
        if (daysSinceTill >= 3)
        {
            m_state = FarmTileState::IDLE;
            m_dayTilled = -1;
            UpdateVisuals();
            std::cout << "[FarmTile] Sol retourné à l'état normal (inactivité)" << std::endl;
            return;
        }
    }

    if (m_crop.has_value() == false) 
        return;

    m_daysWithoutWater++;

    // Calcul de la mort par manque d'eau
    // Jour 3 sans eau = 50%, croissance exponentielle, jour 6 = 100%
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
            m_daysWithoutWater = 0;
            UpdateVisuals();
            return;
        }

        // Probabilité de mort exponentielle entre threshold et maxDays
        // jour 3 = 50%, jour 4 ≈ 65%, jour 5 ≈ 82%, jour 6 = 100%
        float t = static_cast<float>(m_daysWithoutWater - threshold) / static_cast<float>(maxDays - threshold);
        float deathChance = 1.f - std::pow(1.f - t, 2.f); 

        float roll = static_cast<float>(rand()) / RAND_MAX;
        if (roll < deathChance)
        {
            std::cout << "[FarmTile] Plante morte (chance=" << static_cast<int>(deathChance * 100) << "%)" << std::endl;
            m_crop.reset();
            m_state = FarmTileState::TILLED;
            m_growthStage = 0;
            m_daysWithoutWater = 0;
            UpdateVisuals();
            return;
        }
    }

    m_growthStage = ComputeGrowthStage(p_currentDay);

    int maxStage = static_cast<int>(m_crop->m_growthSpritePath.size()) - 1;
    if (m_growthStage >= maxStage && p_currentDay - m_dayPlanted >= m_crop->m_daysToGrow)
    {
        m_state = FarmTileState::READY;
        std::cout << "[FarmTile] " << m_crop->m_name << " prêt à récolter !" << std::endl;
    }
    else
    {
        m_state = m_wateredToday ? FarmTileState::GROWING : FarmTileState::PLANTED;
    }

    UpdateVisuals();
}