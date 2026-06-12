#include "pch.h"
#include "PNJ.h"
#include "2DPendentLib/InputManager.h"
#include <chrono>
#include <condition_variable>
#include <thread>
#include "DialogueManager.h"

#include "Dialogue.h"
/*
debug.CreateText("test", {550,550}, * DialogueManager::Get()->GetDialogueById("test", 1), ColorsNamed::White);

std::vector test = DialogueManager::Get()->GetChoicesById("test", 2);

std::cout << test[0].s_label << std::endl;

for (Dialogue::Choice choice : test)
{
	static int i = 1;
	std::string id = choice.s_label + std::to_string(i++);
	CreateButton({ 100 + 250.f * i, 500 }, { 150, 50 }, "", id, choice.s_label, true)->SetOnClick([id, choice] {
		std::cout << "clicked" << std::endl;
		Debug::Get()->GetText("test")->SetText(*DialogueManager::Get()->GetDialogueById("test", choice.s_target));
		});
}
*/


void PNJ::OnInitialize()
{


	GetScene()->CreateFillRect("dialogBox", Vector2f(GetScene()->GetWindowWidth() / 2, GetScene()->GetWindowHeight() - 100), Vector2i(GetScene()->GetWindowWidth() / 2, 100), Color(0, 0, 255, 255), true);
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);

	float m_rectX = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition({ 0,0 }).GetX();
	float m_rectY = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition({ 0,0 }).GetY();

	m_textX = m_rectX + 10;
	m_textY = m_rectY + 10;

	GetScene()->CreateText("dialog", Vector2f(m_textX, m_textY), "", Color(255, 0, 0, 255), true);
	GetScene()->GetGeometry<Text>("dialog")->SetFontSize(19);
	GetScene()->GetGeometry<Text>("dialog")->SetIsVisible(false);

	GetScene()->CreateText("button", Vector2f(m_textX + 100, m_textY + 50), "", Color(255,255, 255, 255));
	GetScene()->GetGeometry<Text>("button")->SetFontSize(19);
	GetScene()->GetGeometry<Text>("button")->SetIsVisible(false);
	

}

void PNJ::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_SPACE) && m_isInPlayerZone && m_isFinished) {
		EndTalk();
	}
	if (InputManager::Get()->IsKeyboardDown(K_SPACE) && m_index < m_initial.size())
	{
		m_index = m_initial.size();
		m_toSay = m_initial;
		GetScene()->GetGeometry<Text>("dialog")->SetText(m_toSay);
		GetScene()->GetGeometry<Text>("dialog")->SetPosition(Vector2f(m_textX, m_textY), { 0,0.5f });
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
}

void PNJ::Talk(int p_dialogID, int p_choiceID)
{
	m_isTalking = true;
	m_isFinished = false;

	m_choices = DialogueManager::Get()->GetChoicesById(m_dialogPath, p_choiceID);
	m_initial = *DialogueManager::Get()->GetDialogueById(m_dialogPath, p_dialogID);

	m_toSay = "";
	m_index = 0;

	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog")->SetIsVisible(true);

}

void PNJ::EndTalk()
{
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("dialog")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("button")->SetIsVisible(false);
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

			GetScene()->GetGeometry<Text>("dialog")->SetText(m_toSay);
			m_timer = m_interval;
		}
		else
		{
			m_isFinished = true;
		}
		
		GetScene()->GetGeometry<Text>("dialog")->SetPosition(
			Vector2f(m_textX, m_textY),
			{ 0, 0.5f }
		);
	}
}


