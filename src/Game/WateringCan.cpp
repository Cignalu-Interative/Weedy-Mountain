#include "pch.h"
#include "WateringCan.h"

void WateringCan::OnInitialize()
{
	SetMaxLoad(6);
	m_load = m_maxLoad;
}

void WateringCan::Use()
{
	if (m_load <= 0)
		return;

	m_load -= 0.2f;


	// faire en sorte que ca arrose seulement les tuiles concernées
}

void WateringCan::Load()
{
	m_load = m_maxLoad;
}
