#pragma once
#include "2DPendentEngine/Entity.h"
struct Job
{
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

class Player : public Entity
{
private:
	float m_PlayerSpeed = 50.f;

	float m_MaxHp = 100.f;
	float m_Hp = m_MaxHp;

	float m_MaxStamina = 100.f;
	float m_Stamina = m_MaxStamina;

	float m_MaxToxicity = 100.f;
	float m_Toxicity = 0.f;

	std::vector<Job*> m_Jobs;

protected:

	//Fonction pour initialiser le joueur, notamment en définissant sa position, sa dimension, son sprite et son collider
	void OnInitialize() override;
	//Fonction pour gérer les événements liés au joueur, notamment en gérant les entrées du joueur pour le déplacer ou effectuer des actions
	void OnEvent() override;
	//Fonction pour mettre à jour le joueur, notamment en mettant à jour son sprite et en gérant sa logique de jeu
	void OnUpdate() override;
	//Fonction pour gérer les collisions du joueur avec d'autres entités, notamment en gérant les interactions avec les ennemis, les objets ou les éléments du décor
	void OnCollision(Entity* other) override;
	//Fonction pour marquer le joueur pour destruction, elle sera détruite à la fin de la frame en cours
	void OnDestroy() override;

public:

	float GetHp() { return m_Hp; }
	float GetStamina() { return m_Stamina; }
	float GetToxicity() { return m_Toxicity; }
	std::vector<Job*> GetJobs() { return m_Jobs; }

	void SetHp(float Hp) { m_Hp = Hp; }
	void SetStamina(float Stamina) { m_Stamina = Stamina; }
	void SetToxicity(float Toxicity) { m_Toxicity = Toxicity; }


};

