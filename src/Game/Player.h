#pragma once
#include "2DPendentEngine/Entity.h"
#include "CropDefinition.h"
#include "StateMachine.h"


enum class PlayerState
{
	IDLE,
	WALKING,

	WATERING,
	DIGGING,
	MINING,

	COUNT
};


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
	float m_Hp = m_maxHp;

	float m_maxStamina = 100.f;
	float m_Stamina = m_maxStamina;

	float m_maxToxicity = 100.f;
	float m_toxicity = 0.f;

	float m_harvestEffiency = 1.f;
	float m_farmerSpeed = 1.f;
	float m_miningSpeed = 1.f;

	float m_drugsSellQuantity = 0.f;
	float m_drugSellEfficency = 1.f; 
};

class Inventory;
class Player : public Entity
{
private:
	StateMachine<Player> m_stateMachine;

	PlayerStats m_stats;
	std::vector<PlayerJob*> m_jobs;
	Inventory* m_inventory;

	// TEMP - AGRI
	CropDefinition m_selectedCrop = CropCatalog::Wheat(); 
	void HandleFarmAction();

	std::vector<Recipes> m_unlockedRecipes;

	bool m_isUsingToolInHand = false;

	bool m_inputBlocker = false;

	int m_money = 2000000;

protected:


	void OnInitialize() override;
	
	void OnEvent() override;

	void OnUpdate() override;
	
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;


	void OnDestroy() override;

public:
	std::vector<PlayerJob*>& GetJobs() { return m_jobs; }
	PlayerStats& GetStats() { return m_stats; }

	void UpdateJobs();
	void UnlockRecipe(Recipes _toUnlock);
	void SetupStateMachine();

	Inventory* GetInventory() { return m_inventory; }

	int GetMoney() { return m_money; }
	void AddMoney(int p_amount) { m_money += p_amount; }
	void DecreaseMoney(int p_amount) { m_money -= p_amount; }
	void SetIsUsingToolInHand(bool p_isUsing) { m_isUsingToolInHand = p_isUsing; }
	void SetState(PlayerState p_state);

	bool IsRecipeUnlocked(Recipes _toUnlock);
	bool IsUsingToolInHand() { return m_isUsingToolInHand; }

	void LockInputs();
	void UnlockInputs();

	// temp en attendant d'avoir la hotbar et le current item en main
	bool hasSpade = false;
	bool hasPickaxe = false;
	bool hasWateringCan = false;
};

