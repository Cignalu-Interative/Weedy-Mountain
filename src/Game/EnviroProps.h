#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Button.h"
enum class EnviroName
{
	Tree,
	Shrub,
	Grass,
	Rock,
	Stone
};

struct EnviroStats {
	int m_MinSpawn = 0;
	int m_MaxSpawn = 1;
	int m_HitToDestroy = 1;
	//Rarity m_MinToolsRarity = Rarity::Wooden;
};

class EnviroProps: public Button
{
	EnviroName m_Name;
	EnviroStats m_Stats;

protected:
	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnCollision(Entity* other) override;

	void OnDestroy() override;

public:
	EnviroProps() = default;
	void SetType(EnviroName p_name);

};

