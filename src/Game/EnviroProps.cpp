#include "pch.h"
#include "EnviroProps.h"

void EnviroProps::OnInitialize()
{
	switch(m_Name) {
	case EnviroName::Tree:
		m_Stats.m_MinSpawn = 1;
		m_Stats.m_MaxSpawn = 3;
		m_Stats.m_HitToDestroy = 4;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Shrub:
		m_Stats.m_MinSpawn = 2;
		m_Stats.m_MaxSpawn = 4;
		m_Stats.m_HitToDestroy = 2;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Grass:
		m_Stats.m_MinSpawn = 3;
		m_Stats.m_MaxSpawn = 5;
		m_Stats.m_HitToDestroy = 1;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Rock:
		m_Stats.m_MinSpawn = 0;
		m_Stats.m_MaxSpawn = 0;
		m_Stats.m_HitToDestroy = 2;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Stone:
		m_Stats.m_MinSpawn = 0;
		m_Stats.m_MaxSpawn = 0;
		m_Stats.m_HitToDestroy = 4;
		//m_Stats.m_MinToolsRarity
		break;
	}

	SetOnClick([this] {
		m_Stats.m_HitToDestroy--;
		if (m_Stats.m_HitToDestroy <= 0) {
			Destroy();
		}
	});
}
