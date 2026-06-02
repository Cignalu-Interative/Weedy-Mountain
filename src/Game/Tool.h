#pragma once
#include "2DPendentEngine/Entity.h"

enum class Rarity
{
	Wood,
	Stone,
	Iron,
	Diamond,

	Count
};


class Tool : public Entity
{
private:
	float m_maxDurability;
	float m_currentDurability;
	Rarity m_rarity = Rarity::Wood;

public:
	void OnUpdate() override;
	void OnInitialize() override;

	virtual void Use();

	void SetMaxDurability(float maxDurability);


	Rarity GetRarity() { return m_rarity; }
	void SetRarity(Rarity rarity) { m_rarity = rarity; }
};

