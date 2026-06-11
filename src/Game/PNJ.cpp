#include "pch.h"
#include "PNJ.h"
#include "2DPendentLib/InputManager.h"
#include <chrono>
#include <condition_variable>
#include <thread>
void PNJ::OnInitialize()
{
	GetScene()->CreateFillRect("dialogBox", Vector2f(GetScene()->GetWindowWidth() / 2, GetScene()->GetWindowHeight() - 100), Vector2i(GetScene()->GetWindowWidth() / 2, 100), Color(255, 255, 255, 255), true);
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);

	m_rectX = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition().GetX();
	m_rectY = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition().GetY();
	m_rectWidth = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetDimension().GetX();
	m_rectHeight = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetDimension().GetY();

	/*m_dialogBox = GetScene()->CreateEntity<Entity>(Vector2f(640, 620), { GetScene()->GetWindowWidth() / 2,100 }, " ", Shape::Rectangle, true);
	m_dialogBox->ShowCollider(true);
	

	float rectX = m_dialogBox->GetPosition().GetX();
	float rectY = m_dialogBox->GetPosition().GetY();
	float rectWidth = m_dialogBox->GetDimension().GetX();
	float rectHeight = m_dialogBox->GetDimension().GetY();*/


	GetScene()->CreateText("dialog", Vector2f(m_rectX - m_rectWidth / 2.5f, m_rectY - m_rectHeight / 2.5f), "", Color(0, 0, 0, 255), true);
	GetScene()->GetGeometry<Text>("dialog")->SetFontSize(20);
	GetScene()->GetGeometry<Text>("dialog")->SetIsVisible(false);
}

void PNJ::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_e) && m_isInPlayerZone && !m_isTalking)
	{
		Talk("Bonjour KqkqkqkqkqA wsh Gael cmt tu vas  kqkqkqkqkqqkqkqkqkqkqkqkqkq Hihihihihihihih");
	}
	if (InputManager::Get()->IsKeyboardDown(K_SPACE) && m_isInPlayerZone && m_isFinished){
		EndTalk();
	}
	if (InputManager::Get()->IsKeyboardDown(K_SPACE) && m_index < m_initial.size())
	{
		m_toSay = m_initial;
		m_index = m_initial.size();
		GetScene()->GetGeometry<Text>("dialog")->SetText(m_toSay);
		GetScene()->GetGeometry<Text>("dialog")->SetPosition(Vector2f(m_rectX - m_rectWidth / 2.5f, m_rectY - m_rectHeight / 2.5f),{0,0.5f});
		return;
	}
}

void PNJ::OnUpdate()
{
	m_timer -= GetDeltaTime();
	if (!m_isTalking)
		return;
	if (m_isFinished)
		return;
	
	UpdateDialog();
	GetScene()->GetGeometry<Text>("dialog")->SetText(m_toSay);
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
	m_isFinished = false;

	m_initial = p_say;
	m_toSay = "";
	m_index = 0;

	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog")->SetIsVisible(true);

	GetScene()->GetGeometry<Text>("dialog")->SetPosition(
		Vector2f(m_rectX - m_rectWidth / 2.5f, m_rectY - m_rectHeight / 2.5f),
		{ 0, 0 }
	);
}

void PNJ::EndTalk()
{
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("dialog")->SetIsVisible(false);
	m_isTalking = false;
	m_isFinished = true;
	m_toSay = "";
}

void PNJ::UpdateDialog()
{
	m_timer -= GetDeltaTime();

	if (m_timer <= 0.f)
	{
		if (m_index < m_initial.size())
		{
			
			m_toSay += m_initial[m_index];
			m_index++;

			//GetScene()->GetGeometry<Text>("dialog")->SetPosition(GetScene()->GetGeometry<Text>("dialog")->GetPosition() + Vector2f(3.75f, 0.f));
			GetScene()->GetGeometry<Text>("dialog")->SetText(m_toSay);

			m_timer = m_interval;
		}
		else
		{
			m_isFinished = true;
		}
	}
}
