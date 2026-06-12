#pragma once
#include "2DPendentEngine/Entity.h"
#include "CropDefinition.h" 

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
	float m_PlayerSpeed = 10.f;

	float m_MaxHp = 100.f;
	float m_Hp = m_MaxHp;

	float m_MaxStamina = 100.f;
	float m_Stamina = m_MaxStamina;

	float m_MaxToxicity = 100.f;
	float m_Toxicity = 0.f;

	float m_harvestEffiency = 1.f;
	float m_farmerSpeed = 1.f;
	float m_miningSpeed = 1.f;

	float m_drugsSellQuantity = 0.f;
	float m_drugSellEfficency = 1.f; 
};


class Player : public Entity
{
private:
	PlayerStats m_Stats;
	std::vector<PlayerJob*> m_Jobs;


	// TEMP - AGRI
	CropDefinition m_selectedCrop = CropCatalog::Wheat(); 
	void HandleFarmAction();

	std::vector<Recipes> m_unlockedRecipes;
protected:


	void OnInitialize() override;
	
	void OnEvent() override;

	void OnUpdate() override;
	
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnDestroy() override;

public:
	std::vector<PlayerJob*>& GetJobs() { return m_Jobs; }
	PlayerStats& GetStats() { return m_Stats; }

	void UpdateJobs();
	void UnlockRecipe(Recipes _toUnlock);
	bool IsRecipeUnlocked(Recipes _toUnlock);
};

