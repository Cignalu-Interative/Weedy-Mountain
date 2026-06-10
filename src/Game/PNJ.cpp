#include "pch.h"
#include "PNJ.h"
#include "2DPendentLib/InputManager.h"

void PNJ::OnInitialize()
{
	GetScene()->CreateText("dialog", Vector2f(GetPosition().GetX(), GetPosition().GetY() + 20), "", Color(255, 255, 255, 255), false);
	Debug::Get()->GetText("dialog")->SetFontSize(17);
}

void PNJ::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_e) && m_isInPlayerZone && !m_isTalking)
	{
		Talk("Bonjour");
	}
	if (InputManager::Get()->IsKeyboardDown(K_SPACE) && m_isInPlayerZone)
	{
		EndTalk();
	}
}

void PNJ::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox") {
		m_isInPlayerZone = true;
	}
}

void PNJ::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox") {
		m_isInPlayerZone = false;
	}
}

void PNJ::Talk(const std::string& p_say)
{
	m_isTalking = true;
	Debug::Get()->GetText("dialog")->SetText(p_say);
}

void PNJ::EndTalk()
{
	Debug::Get()->GetText("dialog")->SetText("");
	m_isTalking = false;
}
