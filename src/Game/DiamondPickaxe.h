#pragma once
#include "Pickaxe.h"

class DiamondPickaxe : public Pickaxe
{
public :

	//Fonction pour initialiser l'outil, notamment en définissant son sprite
	void OnInitialize() override;
};

