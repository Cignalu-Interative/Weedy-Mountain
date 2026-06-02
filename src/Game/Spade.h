#pragma once
#include "Tool.h"

class Spade : public Tool
{
public : 
	//Fonction pour initialiser l'outil, notamment en définissant son sprite
	void OnInitialize() override;
	//Fonction pour mettre à jour l'outil, notamment en mettant à jour son sprite et en gérant sa logique de jeu
	void OnUpdate() override {}
	//Fonction pour marquer l'outil pour destruction, elle sera détruite à la fin de la frame en cours
	void OnDestroy() override;

	void Use() override {};
};

