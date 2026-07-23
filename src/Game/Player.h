#pragma once
#include "2DPendentEngine/Entity.h"
#include "CropDefinition.h"
#include "StateMachine.h"
#include "GameData.h"

enum class PlayerState
{
	IDLE,
	WALKING,

	WATERING,
	DIGGING,
	MINING,

	COUNT
};

enum class FishingState
{
	Idle,
	Casting,
	Waiting,
	BiteWindow
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

	GameData& gm = GameData::Get();
	// PECHE 
	FishingState m_fishingState = FishingState::Idle;
	Collider* m_fishingZoneCollider = nullptr;

	Vector2f m_facingDirection = { 0.f, 1.f };

	bool m_isMovingThisFrame = false;
	bool m_fishingZoneTouchingWater = false;

	float m_castSyncTimer = 0.f;

	float m_fishingTimer = 0.f;
	float m_fishingTimerTarget = 0.f;
	float m_biteReactionTimer = 0.f;

	static constexpr float FISHING_CAST_DISTANCE = 24.f;   
	static constexpr float FISHING_BITE_WINDOW = 0.5f;

	static constexpr float FISHING_CAST_SYNC_DELAY = (1.f / 120.f) * 2.5f;

	void StartFishing();
	void UpdateFishing();
	void RetractFishingRod();

protected:


	void OnInitialize() override;
	
	void OnEvent() override;

	void OnUpdate() override;
	
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;


	void OnDestroy() override;

public:
	std::vector<PlayerJob>& GetJobs() { return gm.GetPlayerData().m_jobs; }
	PlayerStats GetStats() { return gm.GetPlayerData().m_stats; }

	PlayerJob GetJobByName(JobType p_name);

	void UpdateJobs();
	void UnlockRecipe(Recipes _toUnlock);
	void SetupStateMachine();

	PlayerInventory* GetInventory() { return gm.GetPlayerData().m_playerInventory; }

	int GetMoney() const { return gm.GetPlayerData().m_money; }
	void AddMoney(int p_amount) { gm.GetPlayerData().m_money += p_amount; }
	void DecreaseMoney(int p_amount);

	int GetCash() const { return gm.GetPlayerData().m_cash; }
	void AddCash(int p_amount) { gm.GetPlayerData().m_cash += p_amount; }
	void DecreaseCash(int p_amount);

	void SetIsUsingToolInHand(bool p_isUsing) { m_isUsingToolInHand = p_isUsing; }
	void SetState(PlayerState p_state);

	bool IsRecipeUnlocked(Recipes _toUnlock);
	bool IsUsingToolInHand() { return m_isUsingToolInHand; }

	void LockInputs();
	void UnlockInputs();

	int GetLife() { return gm.GetPlayerData().m_stats.m_hp; }
	int GetMaxLife() { return gm.GetPlayerData().m_stats.m_maxHp; }
	void Heal(int p_amount);
	void TakeDamage(int p_amount);

	int GetStamina() { return gm.GetPlayerData().m_stats.m_stamina; }
	int GetMaxStamina() { return gm.GetPlayerData().m_stats.m_maxStamina; }
	void DecreaseStamina(int p_amount);
	void AddStamina(int p_amount);

	int GetToxicity() { return gm.GetPlayerData().m_stats.m_toxicity; }
	int GetMaxToxicity() { return gm.GetPlayerData().m_stats.m_maxToxicity; }
	void DecreaseToxicity(int p_amount);
	void AddToxicity(int p_amount);

	bool AreInputLocked() { return m_inputBlocker; }

	// temp en attendant d'avoir la hotbar et le current item en main
	bool hasSpade = false;
	bool hasPickaxe = false;
	bool hasWateringCan = false;
};

