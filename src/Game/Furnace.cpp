#include "pch.h"
#include "Furnace.h"
#include "FurnaceUi.h"
#include "SpriteContainer.h"
#include "2DPendentLib/InputManager.h"
#include "SaveManager.h"

void Furnace::OnInitialize()
{
	AddSprite("Furnace", "furnace", GetDimension());
	SyncSprites();
	
	Scene* scene = GetScene();

	m_furnaceUi = scene->CreateEntity<FurnaceUi>(scene->GetWindowCenter(), { 800,600 }, "", Shape::Rectangle, true);
	m_furnaceUi->Close();
	m_furnaceUi->SetDrawLayer(INT_MAX - 200);

	SaveManager::Get().RegisterProvider(this);
}

void Furnace::OnDestroy()
{
	SaveManager::Get().UnregisterProvider(this);
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

void Furnace::Save(nlohmann::json& out)
{
	out["posX"] = GetPosition().GetX();
	out["posY"] = GetPosition().GetY();

	nlohmann::json uiState;
	m_furnaceUi->Save(uiState);
	out["ui"] = uiState;
}

void Furnace::Load(const nlohmann::json& in)
{
	if (in.contains("ui"))
		m_furnaceUi->Load(in["ui"]);
}
