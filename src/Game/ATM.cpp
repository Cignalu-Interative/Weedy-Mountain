#include "pch.h"
#include "ATM.h"
#include "UiATM.h"
#include "SpriteContainer.h"

void ATM::OnInitialize()
{
	m_atmUI = GetScene()->CreateEntity<UiATM>({ GetScene()->GetWindowWidth() / 2.f, GetScene()->GetWindowHeight() / 2.f }, { 1000, 600 }, "ATM_BackGround", Shape::Rectangle, true);
	GetSprite()->ResizeStretched(GetDimension());
	SyncSprites();
}

void ATM::OnUpdate()
{

}

void ATM::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox") {
		m_isInPlayerZone = true;
	}
}

void ATM::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox") {
		m_isInPlayerZone = false;
	}
}

void ATM::Toogle()
{
	if (m_atmUI->IsOpen())
		m_atmUI->Close();
	else
		m_atmUI->Open();
}

bool ATM::IsOpen()
{
	return m_atmUI->IsOpen();
}


