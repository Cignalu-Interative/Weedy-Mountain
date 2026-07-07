#include "pch.h"
#include "ItemOnGround.h"
#include "2DPendentLib/Debug.h"
#include "2dPendentLib/Utils.h"
#include "Player.h"
#include "PlayerInventory.h"

static Uint32 m_id = 0;

void ItemOnGround::OnInitialize()
{
	m_myID = m_id++;

	if (m_id == UINT32_MAX)
		m_id = 0;

	GetScene()->CreateText("item_" + std::to_string(m_myID), GetPosition({ 1, 1 }), "", { 255, 255, 255, 255 }, false, false);
}

void ItemOnGround::OnEvent()
{

}

void ItemOnGround::OnUpdate()
{
	SetDrawLayer(200 + GetCollider()->GetPosition({ 1.f,1.f }).GetY());
	if (m_timer > 0) {
		m_timer -= GetDeltaTime();
	}

	MoveKinematic();

	m_timeToDestroy -= GetDeltaTime();
	if (m_timeToDestroy <= 0) {
		Destroy();
	}
}

void ItemOnGround::OnDestroy()
{
	GetScene()->RemoveGeometry("item_" + std::to_string(m_myID));
}

void ItemOnGround::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() != 3)
		return;

	ItemOnGround* otherItem = static_cast<ItemOnGround*>(_other->GetOwner());

	if (ToDestroy() || otherItem->ToDestroy())
		return;

	if (m_ItemInfo.GetID() != otherItem->m_ItemInfo.GetID())
		return;

	if (m_myID < otherItem->m_myID)
	{
		m_ItemInfo.SetAmount(m_ItemInfo.GetAmount() + otherItem->m_ItemInfo.GetAmount());
		Text* text = GetScene()->GetGeometry<Text>("item_" + std::to_string(m_myID));
		if(text) text->SetText(std::to_string(m_ItemInfo.GetAmount()));
		otherItem->Destroy();
		m_timeToDestroy = 300.f;
	}
}

void ItemOnGround::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() != 1 || this->m_toDestroy)
		return;

	if (_other->GetID() == "interact" && m_timer <= 0) {
		float diffX = _other->GetPosition().GetX() - GetPosition().GetX();
		float diffY = _other->GetPosition().GetY() - GetPosition().GetY();
		SetDirection({ diffX / Utils::Distance(GetPosition(),_other->GetPosition()), diffY / Utils::Distance(GetPosition(),_other->GetPosition()) });

		Text* text = GetScene()->GetGeometry<Text>("item_" + std::to_string(m_myID));
		if (text) {
			text->SetPosition(this->GetPosition({1,1}));
		}
		SetResetDirection(false);
		SetSpeed(15);
	}


	if (_other->GetID() == "default" && ToDestroy() == false) {
		std::cout << "Item Collected" << std::endl;
		static_cast<Player*>(GetScene()->GetEntityByTag(1))->GetInventory()->AddItem(m_ItemInfo.GetID(), m_ItemInfo.GetAmount(), false);
		Destroy();
	}
}

void ItemOnGround::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetID() == "interact") {
		SetSpeed(0);
		SetResetDirection(true);
	}
}
