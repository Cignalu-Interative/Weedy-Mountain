#pragma once
#include "2DPendentEngine/Entity.h"

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

struct PlayerStats {
	float m_PlayerSpeed = 50.f;

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

	bool m_canBuyWeedSeed = false;
	bool m_canBuyCannabisSeed = false;

	bool m_isKnifeUnlocked = false;
	bool m_isSwordUnlocked = false;
	bool m_isGlockUnlocked = false;
	bool m_isRifleUnlocked = false;

	bool m_isStoneToolsUnlocked = false;
	bool m_isIronToolsUnlocked = false;
	bool m_isGoldToolsUnlocked = false;
	bool m_isDiamondToolsUnlocked = false;

	bool m_isBandageL1Unlocked = false;
	bool m_isBandageL2Unlocked = false;
	bool m_isEnergizingL1Unlocked = false;
	bool m_isEnergizingL2Unlocked = false;

	bool m_isFertilizerL1Unlocked = false;
	bool m_isFertilizerL2Unlocked = false;
	bool m_isAutoWateringL1Unlocked = false;
	bool m_isAutoWateringL2Unlocked = false;

};


class Player : public Entity
{
private:
	PlayerStats m_Stats;
	std::vector<PlayerJob*> m_Jobs;
protected:

	//Fonction pour initialiser le joueur, notamment en définissant sa position, sa dimension, son sprite et son collider
	void OnInitialize() override;
	//Fonction pour gérer les événements liés au joueur, notamment en gérant les entrées du joueur pour le déplacer ou effectuer des actions
	void OnEvent() override;
	//Fonction pour mettre à jour le joueur, notamment en mettant à jour son sprite et en gérant sa logique de jeu
	void OnUpdate() override;
	//Fonction pour gérer les collisions du joueur avec d'autres entités, notamment en gérant les interactions avec les ennemis, les objets ou les éléments du décor
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	//Fonction pour marquer le joueur pour destruction, elle sera détruite à la fin de la frame en cours
	void OnDestroy() override;

public:
	std::vector<PlayerJob*>& GetJobs() { return m_Jobs; }
	PlayerStats& GetStats() { return m_Stats; }

	void UpdateJobs();

};

