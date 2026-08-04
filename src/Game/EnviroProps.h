#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Button.h"
#include "ItemType.h"
#include "Tool.h"
#include <variant>
#include "EnviroSpawn.h"
#include "ItemStack.h"
#include "Tile.h"

struct EnviroStats {
	int m_hitToDestroy = 1;
	ToolRarity m_minToolsRarity = ToolRarity::Wooden;
	std::vector<ToolType> m_toolType;
	Tile m_position;
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
	bool m_silentDestroy = false;

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

	void SetTile(const Tile& tile) { m_stats.m_position = tile; }

	EnviroStats* GetStats() { return &m_stats; }

	bool CanBeHitBy(ToolType toolType, ToolRarity toolRarity);

	bool TryHit(ToolType toolType, ToolRarity toolRarity);
	const Tile& GetTile() const { return m_stats.m_position; }

	void DestroyWithoutLoot();
};

