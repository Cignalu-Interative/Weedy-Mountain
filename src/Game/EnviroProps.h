#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Button.h"
#include "Tool.h"
#include "EnviroSpawn.h"

struct EnviroStats {
	int m_HitToDestroy = 1;
	Rarity m_MinToolsRarity = Rarity::Wooden;
	std::vector<ToolType> m_ToolType;
};

class EnviroProps: public Entity
{
	EnviroSpawnName m_Name = EnviroSpawnName::Count;
	EnviroStats m_Stats;

	bool m_isInPlayerZone = false;

protected:
	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnDestroy() override;

public:
	EnviroProps() = default;
	void SetType(EnviroSpawnName p_name);


};

