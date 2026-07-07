#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Button.h"
#include "Tool.h"
#include "EnviroSpawn.h"
#include "ItemStack.h"
#include "FarmTile.h"

struct EnviroStats {
	int m_hitToDestroy = 1;
	Rarity m_minToolsRarity = Rarity::Wooden;
	std::vector<ToolType> m_toolType;
	FarmTile* m_position;
};

struct EnviroLoots {
	ItemStack m_item;
	int m_minDrop;
	int m_maxDrop;
};

class EnviroProps: public Entity
{
	EnviroSpawnName m_name = EnviroSpawnName::Count;
	EnviroStats m_stats;
	std::vector<EnviroLoots*> m_loots;

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

	EnviroSpawnName GetType() { return m_name; }

	void SetFarmTile(FarmTile* position) { m_stats.m_position = position; }

	FarmTile* GetFarmTile() { return m_stats.m_position; }
};

