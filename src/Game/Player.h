#pragma once
#include "2DPendentEngine/Entity.h"
#include "CropDefinition.h"
#include "StateMachine.h"
#include "PlayerData.h"

enum class PlayerState
{
	IDLE,
	WALKING,

	WATERING,
	DIGGING,
	MINING,

	COUNT
};



class Inventory;
class PlayerInventory;
class Hotbar;
class Player : public Entity
{
private:
	StateMachine<Player> m_stateMachine;

	bool m_inputBlocker = false;
	bool m_isUsingToolInHand = false;

	// TEMP - AGRI
	CropDefinition m_selectedCrop = CropCatalog::Wheat();
	void HandleFarmAction();

	PlayerData m_playerData;

protected:


	void OnInitialize() override;
	
	void OnEvent() override;

	void OnUpdate() override;
	
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;


	void OnDestroy() override;

public:
	std::vector<PlayerJob>& GetJobs() { return m_playerData.m_jobs; }
	PlayerStats GetStats() { return m_playerData.m_stats; }

	PlayerJob GetJobByName(JobType p_name);


	void UpdateJobs();
	void UnlockRecipe(Recipes _toUnlock);
	void SetupStateMachine();

	PlayerInventory GetInventory() { return m_playerData.m_playerInventory; }

	int GetMoney() const { return m_playerData.m_money; }
	void AddMoney(int p_amount) { m_playerData.m_money += p_amount; }
	void DecreaseMoney(int p_amount);

	int GetCash() const { return m_playerData.m_cash; }
	void AddCash(int p_amount) { m_playerData.m_cash += p_amount; }
	void DecreaseCash(int p_amount);

	void SetIsUsingToolInHand(bool p_isUsing) { m_isUsingToolInHand = p_isUsing; }
	void SetState(PlayerState p_state);

	bool IsRecipeUnlocked(Recipes _toUnlock);
	bool IsUsingToolInHand() { return m_isUsingToolInHand; }

	void LockInputs();
	void UnlockInputs();

	int GetLife() { return m_playerData.m_stats.m_hp; }
	int GetMaxLife() { return m_playerData.m_stats.m_maxHp; }
	void Heal(int p_amount);
	void TakeDamage(int p_amount);

	int GetStamina() { return m_playerData.m_stats.m_stamina; }
	int GetMaxStamina() { return m_playerData.m_stats.m_maxStamina; }
	void DecreaseStamina(int p_amount);
	void AddStamina(int p_amount);

	int GetToxicity() { return m_playerData.m_stats.m_toxicity; }
	int GetMaxToxicity() { return m_playerData.m_stats.m_maxToxicity; }
	void DecreaseToxicity(int p_amount);
	void AddToxicity(int p_amount);

	// temp en attendant d'avoir la hotbar et le current item en main
	bool hasSpade = false;
	bool hasPickaxe = false;
	bool hasWateringCan = false;
};

