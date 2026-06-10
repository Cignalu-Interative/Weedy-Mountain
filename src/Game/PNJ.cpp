#include "pch.h"
#include "PNJ.h"

void PNJ::OnInitialize()
{

}

void PNJ::OnEvent()
{

}

void PNJ::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox") {
		m_isInPlayerZone = true;
	}
}

void PNJ::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{

}
