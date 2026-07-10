#pragma once
#include "PlayerInventory.h"
#include "Inventory.h"
#include "Hotbar.h"


enum class JobType
{
	Gatherer,
	Farmer,
	Miner,
	Killer,
	Dealer,
	Count
};


struct PlayerJob
{
	JobType m_name;
	int m_lvl = 1;
	float m_exp = 0.f;
	float m_expNeeded = 100.f;

	void levelUp() {
		if (m_exp >= m_expNeeded) {
			m_lvl++;
			m_exp -= m_expNeeded;
		}
		m_expNeeded = 100.f * std::exp(m_lvl * 0.25f);
	}
};

enum class Recipes
{
	WeedSeed,
	CannabisSeed,

	Knife,
	Sword,
	Glock,
	Rifle,

	StoneTools,
	IronTools,
	GoldTools,
	DiamondTools,

	BandageL1,
	BandageL2,
	EnergizingL1,
	EnergizingL2,

	FertilizerL1,
	FertilizerL2,
	AutoWateringL1,
	AutoWateringL2
};

struct PlayerStats {
	float m_playerSpeed = 50.f;

	float m_maxHp = 100.f;
	float m_hp = m_maxHp;

	float m_maxStamina = 100.f;
	float m_stamina = m_maxStamina;

	float m_maxToxicity = 100.f;
	float m_toxicity = 0.f;

	float m_harvestEffiency = 1.f;
	float m_farmerSpeed = 1.f;
	float m_miningSpeed = 1.f;

	float m_drugsSellQuantity = 0.f;
	float m_drugSellEfficency = 1.f;

};


struct PlayerData
{
	PlayerStats m_stats;
	PlayerInventory m_playerInventory;

	std::vector<PlayerJob> m_jobs;
	std::vector<Recipes> m_unlockedRecipes;


	int m_money = 200;
	int m_cash = 0;

};