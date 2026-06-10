#pragma once
#include "ItemStack.h"
#include "2DPendentEngine/Entity.h"
static Uint32 m_id = 1;

class ItemOnGround: public Entity
{
	ItemStack* m_ItemInfo = nullptr;
	int m_myID = m_id++;
public:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;

	void SetItem(ItemStack* _ItemInfo) { m_ItemInfo = _ItemInfo; };
};