#pragma once
#include "2DPendentEngine/Entity.h"

enum class Rarity
{
	Wooden,
	Stone,
	Iron,
	Diamond,

	Count
};
enum class ToolType
{
	Axe,
	Hoe,
	Pickaxe,
	Spade,
	WateringCan,

	Count
};

class Tool : public Entity
{
private:
	float m_energyCost = 0;
	float m_timer = 0;
	float m_useTime = 0;
	float m_load = 0;
	float m_maxLoad;


	Rarity m_rarity = Rarity::Wooden;
	ToolType m_type; 


	void SetUseTime(float _time) { m_useTime = _time; }
public:
	void OnUpdate() override;
	void OnInitialize() override;

	void Use();
	void Load();

	Rarity GetRarity() { return m_rarity; }
	float GetEnergyCost() { return m_energyCost; }
	ToolType GetType() { return m_type; }
	float GetUseTime() { return m_useTime; }

	void SetRarity(Rarity _rarity) { m_rarity = _rarity; }
	void SetEnergyCost(float cost) { m_energyCost = cost; }
	void SetType(ToolType _type) { m_type = _type; }
	void SetMaxLoad(float _max);
	
};

