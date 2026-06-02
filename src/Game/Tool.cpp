#include "pch.h"
#include "Tool.h"

void Tool::SetMaxDurability(float maxDurability)
{
	m_maxDurability = maxDurability;
	m_currentDurability = maxDurability;
}

void Tool::OnUpdate()
{
	if (m_currentDurability <= 0)
		Destroy();
}

void Tool::OnInitialize()
{
	switch (GetRarity())
	{
	case Rarity::Wood:
		SetMaxDurability(100);
	case Rarity::Stone:
		SetMaxDurability(120);
	case Rarity::Iron:
		SetMaxDurability(140);
	case Rarity::Diamond:
		SetMaxDurability(160);
	}
}

void Tool::Use()
{
	m_currentDurability -= 1;
}