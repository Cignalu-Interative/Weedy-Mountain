#include "pch.h"
#include "ItemOnGround.h"


void ItemOnGround::OnInitialize()
{
}

void ItemOnGround::OnEvent()
{
}

void ItemOnGround::OnUpdate()
{
}

void ItemOnGround::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 3 && _other->GetID() == "itemZone") {
		m_ItemInfo->SetAmount(m_ItemInfo->GetAmount() + 1);
		_other->GetOwner()->Destroy();
	}
}