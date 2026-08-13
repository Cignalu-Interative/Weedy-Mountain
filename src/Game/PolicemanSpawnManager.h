#pragma once
#include "Policeman.h"

class PolicemanSpawnManager
{
private :

	Policeman* m_policeman;
	Policeman* m_policeman2;
	Policeman* m_policeman3;
	Policeman* m_policeman4;
	Policeman* m_policeman5;

	bool m_isDespawn = true;

public :

	static PolicemanSpawnManager& Get();

	void SpawnPoliceman();
	void DespawnPoliceman();
	
	void Update();
};

