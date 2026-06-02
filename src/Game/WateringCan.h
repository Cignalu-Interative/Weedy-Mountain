#pragma once
#include "Tool.h"
class WateringCan : public Tool
{
private:
	float m_maxLoad;
	float m_load;

public:
	//Fonction pour initialiser l'outil, notamment en définissant son sprite
	void OnInitialize() override;
	//Fonction pour mettre à jour l'outil, notamment en mettant à jour son sprite et en gérant sa logique de jeu
	void OnUpdate() override {}


	void Use();
	void Load();
	void SetMaxLoad(float max) { m_maxLoad = max; }
};

