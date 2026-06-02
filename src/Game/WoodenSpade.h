#pragma once
#include "Spade.h"
class WoodenSpade :public Spade
{
public : 
	//Fonction pour initialiser l'outil, notamment en définissant son sprite
	void OnInitialize() override;
};

