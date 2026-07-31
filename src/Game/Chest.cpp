#include "pch.h"
#include "Chest.h"
#include "UiChest.h"
#include "2DPendentLib/InputManager.h"

void Chest::OnInitialize()
{
	m_uiChest = GetScene()->CreateEntity<UiChest>(GetScene()->GetWindowCenter(), { 800,700 }, "", Shape::Rectangle, true);
	m_uiChest->Close();
}

void Chest::OnEvent()
{
	if (m_isColliding && InputManager::Get()->IsKeyboardDown(K_E))
		m_uiChest->Open();

	if (InputManager::Get()->IsKeyboardDown(K_ESCAPE))
		m_uiChest->Close();
}

void Chest::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox")
		m_isColliding = true;
}

void Chest::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox")
		m_isColliding = false;
}