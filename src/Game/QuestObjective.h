#pragma once
#include <string>
#include "QuestEvent.h"
#include "2DPendentLib/json.hpp"

class QuestObjective
{
protected:

	int m_requiredAmount = 1;
	int m_currentAmount = 0;
	bool m_isCompleted = false;

	std::string m_iconTextureID;

	void AddProgress(int p_amount)
	{
		if (m_isCompleted)
			return;

		m_currentAmount += p_amount;
		if (m_currentAmount >= m_requiredAmount)
		{
			m_currentAmount = m_requiredAmount;
			m_isCompleted = true;
		}
	}

public:

	QuestObjective(const std::string& p_iconTextureID, int p_requiredAmount) : m_iconTextureID(p_iconTextureID), m_requiredAmount(p_requiredAmount) {}

	virtual ~QuestObjective() = default;

	virtual void OnEvent(const QuestEvent& p_event) = 0;

	virtual std::string GetDescription() const = 0;

	bool IsCompleted() const { return m_isCompleted; }

	int GetCurrentAmount() const { return m_currentAmount; }
	int GetRequiredAmount() const { return m_requiredAmount; }

	const std::string& GetIconTextureID() const { return m_iconTextureID; }

	virtual void Save(nlohmann::json& p_out) const
	{
		p_out["current"] = m_currentAmount;
		p_out["completed"] = m_isCompleted;
	}

	virtual void Load(const nlohmann::json& p_in)
	{
		m_currentAmount = p_in.value("current", 0);
		m_isCompleted = p_in.value("completed", false);
	}
};

class ClearTerrainObjective : public QuestObjective
{
public:
	ClearTerrainObjective(int p_requiredAmount = 1, const std::string& p_iconTextureID = "icon_terrain") : QuestObjective(p_iconTextureID, p_requiredAmount) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type != QuestEventType::TerrainCleared)
			return;

		AddProgress(p_event.m_amount);
	}

	std::string GetDescription() const override { return "Defricher une zone de terrain"; }
};

class DigHoleObjective : public QuestObjective
{
public:
	DigHoleObjective(int p_requiredAmount = 1, const std::string& p_iconTextureID = "Iron_Shovel") : QuestObjective(p_iconTextureID, p_requiredAmount) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type != QuestEventType::HoleDug)
			return;

		AddProgress(p_event.m_amount);
	}

	std::string GetDescription() const override { return "Creuser un trou avec la pelle"; }
};

class PlantSeedObjective : public QuestObjective
{
	std::string m_cropID;

public:
	PlantSeedObjective(int p_requiredAmount = 1, const std::string& p_cropID = "", const std::string& p_iconTextureID = "icon_seed") : QuestObjective(p_iconTextureID, p_requiredAmount), m_cropID(p_cropID) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type != QuestEventType::SeedPlanted)
			return;

		if (m_cropID.empty() == false && p_event.m_itemID != m_cropID)
			return;

		AddProgress(p_event.m_amount);
	}

	std::string GetDescription() const override { return "Planter des graines"; }
};

class WaterAndHarvestObjective : public QuestObjective
{
	int m_requiredWaterCount = 1;
	int m_currentWaterCount = 0;
	bool m_waterStepCompleted = false;

	std::string m_cropID; 

public:
	WaterAndHarvestObjective(int p_requiredWaterCount, int p_requiredHarvestAmount, const std::string& p_cropID = "", const std::string& p_iconTextureID = "icon_watering_can") : QuestObjective(p_iconTextureID, p_requiredHarvestAmount), m_requiredWaterCount(p_requiredWaterCount), m_cropID(p_cropID) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type == QuestEventType::PlantWatered)
		{
			if (m_waterStepCompleted)
				return;

			if (m_cropID.empty() == false && p_event.m_itemID != m_cropID)
				return;

			m_currentWaterCount += p_event.m_amount;

			if (m_currentWaterCount >= m_requiredWaterCount)
			{
				m_currentWaterCount = m_requiredWaterCount;
				m_waterStepCompleted = true;
			}
			return;
		}

		if (p_event.m_type == QuestEventType::PlantHarvested)
		{
			if (m_waterStepCompleted == false)
				return;

			if (m_cropID.empty() == false && p_event.m_itemID != m_cropID)
				return;

			AddProgress(p_event.m_amount);
		}
	}

	bool IsWaterStepCompleted() const { return m_waterStepCompleted; }
	int GetCurrentWaterCount() const { return m_currentWaterCount; }
	int GetRequiredWaterCount() const { return m_requiredWaterCount; }

	std::string GetDescription() const override
	{
		return "Arroser la plante " + std::to_string(m_requiredWaterCount) + " fois avant de pouvoir la recolter";
	}

	void Save(nlohmann::json& p_out) const override
	{
		QuestObjective::Save(p_out);
		p_out["waterCurrent"] = m_currentWaterCount;
		p_out["waterCompleted"] = m_waterStepCompleted;
	}

	void Load(const nlohmann::json& p_in) override
	{
		QuestObjective::Load(p_in);
		m_currentWaterCount = p_in.value("waterCurrent", 0);
		m_waterStepCompleted = p_in.value("waterCompleted", false);
	}
};

class SellItemObjective : public QuestObjective
{
	std::string m_itemID;

public:
	SellItemObjective(const std::string& p_itemID, int p_requiredAmount, const std::string& p_iconTextureID = "") : QuestObjective(p_iconTextureID.empty() ? p_itemID : p_iconTextureID, p_requiredAmount), m_itemID(p_itemID) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type != QuestEventType::ItemSold)
			return;

		if (p_event.m_itemID != m_itemID)
			return;

		AddProgress(p_event.m_amount);
	}

	std::string GetDescription() const override
	{
		return "Vendre " + std::to_string(GetRequiredAmount()) + "x " + m_itemID;
	}
};

class ClaimRewardObjective : public QuestObjective
{
public:
	ClaimRewardObjective(const std::string& p_iconTextureID = "icon_reward") : QuestObjective(p_iconTextureID, 1) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type != QuestEventType::RewardClaimed)
			return;

		AddProgress(1);
	}

	std::string GetDescription() const override { return "Recuperer la recompense dans le menu Jobs"; }
};

class CraftItemObjective : public QuestObjective
{
	std::string m_itemID; 

public:
	CraftItemObjective(int p_requiredAmount = 1, const std::string& p_itemID = "", const std::string& p_iconTextureID = "icon_craft") : QuestObjective(p_iconTextureID, p_requiredAmount), m_itemID(p_itemID) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type != QuestEventType::ItemCrafted)
			return;

		if (m_itemID.empty() == false && p_event.m_itemID != m_itemID)
			return;

		AddProgress(p_event.m_amount);
	}

	std::string GetDescription() const override { return "Craft un item"; }
};

class CollectItemObjective : public QuestObjective
{
	std::string m_itemID;

public:
	CollectItemObjective(const std::string& p_itemID, int p_requiredAmount) : QuestObjective(p_itemID, p_requiredAmount), m_itemID(p_itemID) {}

	void OnEvent(const QuestEvent& p_event) override
	{
		if (p_event.m_type != QuestEventType::ItemCollected)
			return;

		if (p_event.m_itemID != m_itemID)
			return;

		AddProgress(p_event.m_amount);
	}

	std::string GetDescription() const override
	{
		return "Recolter " + std::to_string(GetRequiredAmount()) + "x " + m_itemID;
	}
};