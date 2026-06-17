#pragma once
#include "ItemStack.h"
#include "2DPendentEngine/Entity.h"

class ItemOnGround: public Entity
{
	ItemStack m_ItemInfo;
	int m_myID;
protected:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;

	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;
public:

	void SetItem(ItemStack _ItemInfo) { m_ItemInfo = _ItemInfo; };

	ItemStack GetItemInfo() { return m_ItemInfo; }
};