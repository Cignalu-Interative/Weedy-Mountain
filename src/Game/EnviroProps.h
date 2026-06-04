#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Button.h"
#include "Tool.h"
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
	Rarity m_MinToolsRarity = Rarity::Wooden;
	std::vector<ToolType> m_ToolType;
};

class EnviroProps: public Button
{
	EnviroName m_Name;
	EnviroStats m_Stats;

	bool m_isInPlayerZone = false;

protected:
	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnDestroy() override;

public:
	EnviroProps() = default;
	void SetType(EnviroName p_name);

};

