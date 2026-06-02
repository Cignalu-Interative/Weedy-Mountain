#pragma once
#include "2DPendentEngine/Entity.h"

enum class Rarity
{
	Wooden,
	Stone,
	Iron,
	Diamond
};

class Tool : public Entity
{
private:
	float m_energyCost = 0;
	Rarity m_rarity = Rarity::Wooden;
	
public:
	void OnUpdate() override;
	void OnInitialize() override;

	Rarity GetRarity() { return m_rarity; }
	void SetRarity(Rarity rarity) { m_rarity = rarity; }

	float GetEnergyCost() { return m_energyCost; }
	void SetEnergyCost(float cost) { m_energyCost = cost; }

};

