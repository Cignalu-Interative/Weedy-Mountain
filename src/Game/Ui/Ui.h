#pragma once
#include"Entity.h" 
#include "GameData.h"

class Ui : public Entity
{
protected : 

	GameData& m_gameData = GameData::Get();

	float m_currentValue = 0;
	float m_maxValue = 0;

public :

};

