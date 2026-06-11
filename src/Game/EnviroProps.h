#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Button.h"
#include "Tool.h"
#include "EnviroSpawn.h"
#include "ItemStack.h"

struct EnviroStats {
	int m_HitToDestroy = 1;
	Rarity m_MinToolsRarity = Rarity::Wooden;
	std::vector<ToolType> m_ToolType;
};

struct EnviroLoots {
	ItemStack m_item;
	int m_MinDrop;
	int m_MaxDrop;
};

class EnviroProps: public Entity
{
	EnviroSpawnName m_Name = EnviroSpawnName::Count;
	EnviroStats m_Stats;
	std::vector<EnviroLoots*> m_Loots;

	bool m_isInPlayerZone = false;

protected:
	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnDestroy() override;

	void CreateItem(int lootPos);

public:
	EnviroProps() = default;
	void SetType(EnviroSpawnName p_name);


};

