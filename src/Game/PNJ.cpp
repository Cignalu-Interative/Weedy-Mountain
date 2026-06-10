#include "pch.h"
#include "PNJ.h"
#include "2DPendentLib/InputManager.h"

void PNJ::OnInitialize()
{
	GetScene()->CreateFillRect("dialogBox", Vector2f(GetScene()->GetWindowWidth()/2, GetScene()->GetWindowHeight() - 100), Vector2i(200, 100), Color(255, 255, 255, 255), true);
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);

	float rectX = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition().GetX();
	float rectY = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition().GetY();
	float rectWidth = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetDimension().GetX();
	float rectHeight = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetDimension().GetY();

	GetScene()->CreateText("dialog", Vector2f(rectX - rectWidth  /3, rectY - rectHeight / 2.5f), "", Color(0, 0, 0, 255), true);
	Debug::Get()->GetText("dialog")->SetFontSize(17);
	Debug::Get()->GetText("dialog")->SetIsVisible(false);
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
	
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(true);
	Debug::Get()->GetText("dialog")->SetText(p_say);
	Debug::Get()->GetText("dialog")->SetIsVisible(true);
}

void PNJ::EndTalk()
{
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);
	Debug::Get()->GetText("dialog")->SetIsVisible(false);
	m_isTalking = false;
}
