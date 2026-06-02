#include "pch.h"
#include "EnviroProps.h"

void EnviroProps::OnInitialize()
{
	if (m_Name == EnviroName::Tree) {
		m_Stats.m_MinSpawn = 1;
		m_Stats.m_MaxSpawn = 3;
		m_Stats.m_HitToDestroy = 4;
	}
}
