#include "pch.h"
#include "Furnace.h"
#include "FurnaceUi.h"
#include "SpriteContainer.h"
#include "2DPendentLib/InputManager.h"

void Furnace::OnInitialize()
{
	AddSprite("Furnace", "furnace", GetDimension());
	SyncSprites();
	
	Scene* scene = GetScene();

	m_furnaceUi = scene->CreateEntity<FurnaceUi>(scene->GetWindowCenter(), { 800,600 }, "", Shape::Rectangle, true);
	m_furnaceUi->Close();
	m_furnaceUi->SetDrawLayer(INT_MAX - 200);
}

void Furnace::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_ESCAPE))
		m_furnaceUi->Close();
	if (InputManager::Get()->IsKeyboardDown(K_E) && m_isColliding)
		m_furnaceUi->Open();
}

void Furnace::OnUpdate()
{
}

void Furnace::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox")
		m_isColliding = true;
}

void Furnace::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox")
		m_isColliding = false;
}
