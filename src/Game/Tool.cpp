#include "pch.h"
#include "Tool.h"

void Tool::OnUpdate()
{
	m_timer -= GetDeltaTime();
	if (m_timer <= 0)
		m_timer = 0;

}

void Tool::OnInitialize()
{
	m_load = m_maxLoad;
	switch (m_rarity)
	{

	case Rarity::Wooden : 
		SetUseTime(2);
		break;
	case Rarity::Stone : 
		SetUseTime(1.75);
		break;
	case Rarity::Iron : 
		SetUseTime(1.25);
		break;
	case Rarity::Gold : 
		SetUseTime(1);
		break;
	case Rarity::Diamond : 
		SetUseTime(0.4);
		break;
	}
}

void Tool::Use()
{
	switch (m_type)
	{
	case ToolType::WateringCan : 
		if (m_load <= 0)
		{
			std::cout << " there is no water" << std::endl;
			break;
		}
		if (m_timer > 0)
		{
			std::cout << "wait cd" << std::endl;
			break;
		}

		m_load -= 0.2f;
		std::cout << "using WateringCan" << std::endl;
		m_timer = m_useTime;
		// substract energy cost from player energy
		// GetPlayerEnergy() -= GetEnergyCost()  sum like that
		break;

	default : 
		if (m_timer > 0)
		{
			std::cout << "wait cd you are too fast" << std::endl;
			break;
		}
		std::cout << "using tool" << std::endl;
		m_timer = m_useTime;
		// substract energy cost from player energy
		// GetPlayerEnergy() -= GetEnergyCost()  sum like that 
		break;
	}
	
}


void Tool::Load()
{
	m_load = m_maxLoad;
}

void Tool::SetMaxLoad(float _max)
{
	m_maxLoad = _max;
}