#include "pch.h"
#include "PNJ.h"
#include "2DPendentLib/InputManager.h"
#include <chrono>
#include <condition_variable>
#include <thread>
#include "DialogueManager.h"
#include "Dialogue.h"
#include <sstream>

void PNJ::OnInitialize()
{
	m_scene = GetScene();
	m_scene->CreateFillRect("dialogBox", Vector2f(GetScene()->GetWindowWidth() / 2, GetScene()->GetWindowHeight() - 100), Vector2i(GetScene()->GetWindowWidth() / 1.9f, 100), Color(255, 255, 255, 255), true);
	m_scene->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);

	m_dialogBoxPos = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition({ 0,0 });
	m_dialogBoxDim = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetDimension();
	m_pressSpacePos = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition({ 1,1 });

	float m_rectX = m_dialogBoxPos.GetX();
	float m_rectY = m_dialogBoxPos.GetY();

	m_textX = m_rectX + 10;
	m_textY = m_rectY + 10;

	m_maxTextHeight = m_dialogBoxDim.GetY() - 15.f;

	m_scene->CreateText("dialog", Vector2f(m_textX, m_textY), "", ColorsNamed::Black, true);
	m_scene->GetGeometry<Text>("dialog")->SetFontSize(22);
	m_scene->GetGeometry<Text>("dialog")->SetIsVisible(false);
	m_scene->GetGeometry<Text>("dialog")->SetIsWrapped(true);
	m_scene->GetGeometry<Text>("dialog")->SetWrapWidth( m_dialogBoxDim.GetX() * 0.9);

	m_scene->CreateText("pressSpace", Vector2f(m_pressSpacePos.GetX() - 10, m_pressSpacePos.GetY() - 10), "Press SPACE to continue", Color(0, 0, 0, 200), true);
	m_scene->GetGeometry<Text>("pressSpace")->SetFontSize(17);
	m_scene->GetGeometry<Text>("pressSpace")->SetPosition({ m_pressSpacePos.GetX() - 10, m_pressSpacePos.GetY() }, { 1,1 });
	m_scene->GetGeometry<Text>("pressSpace")->SetIsVisible(false);

}

void PNJ::OnEvent()
{
	bool spacePressed = InputManager::Get()->IsKeyboardDown(K_SPACE);

	if (!spacePressed || !m_isInPlayerZone || !m_isTalking)
		return;
	
	if (m_isFinished)
	{
		EndTalk();
		return;
	}
	
	if (m_isPageFull)
	{
		m_toSay.clear();
		m_scene->GetGeometry<Text>("dialog")->SetText("");
		m_isPageFull = false;
		return;
	}

	SkipTalk();
}

void PNJ::OnUpdate()
{
	m_timer -= GetDeltaTime();

	SetDrawLayer(200 + GetCollider()->GetPosition({ 1.f,1.f }).GetY());

	if (!m_isTalking)
		return;
	if (m_isFinished)
		return;

	UpdateDialog();
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
	m_toSay.clear();
	m_scene->GetGeometry<Text>("dialog")->SetText("");

	m_isPageFull = false;
	m_isTalking = true;
	m_isFinished = false;

	m_initial = p_say;
	m_index = 0;

	m_scene->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(true);
	m_scene->GetGeometry<Text>("dialog")->SetIsVisible(true);
	m_scene->GetGeometry<Text>("pressSpace")->SetIsVisible(true);
}

void PNJ::Talk(int p_dialogID)
{
	m_toSay.clear();
	m_scene->GetGeometry<Text>("dialog")->SetText("");

	m_isPageFull = false;
	m_isTalking = true;
	m_isFinished = false;

	m_choices = DialogueManager::Get()->GetChoicesById(m_dialogPath, p_dialogID);

	if (!m_choices.empty())
	{
		m_scene->GetGeometry<Text>("pressSpace")->SetIsVisible(false);
		int i = 1;
		for (Dialogue::Choice choice : m_choices)
		{
			std::string id = choice.s_label + std::to_string(i++);
			Button* button = m_scene->CreateButton({ 250.f * i, m_pressSpacePos.GetY() + 40 }, { 50, 25 }, "", id, choice.s_label, true);
			button->SetOnRightClick([this, choice] {
				for (int i = 0; i < m_buttons.size(); i++)
				{
					m_buttons[i]->Destroy();
				}
				m_buttons.clear();
				Talk(choice.s_target);
				});
			m_buttons.push_back(button);
		}
	}

	m_initial = *DialogueManager::Get()->GetDialogueById(m_dialogPath, p_dialogID);

	m_index = 0;

	m_scene->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(true);
	m_scene->GetGeometry<Text>("dialog")->SetIsVisible(true);
	m_scene->GetGeometry<Text>("pressSpace")->SetIsVisible(true);

}

void PNJ::EndTalk()
{

	m_scene->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);
	m_scene->GetGeometry<Text>("dialog")->SetIsVisible(false);
	m_scene->GetGeometry<Text>("dialog")->SetText("");
	m_initial = "";
	m_toSay = "";
	m_scene->GetGeometry<Text>("pressSpace")->SetIsVisible(false);

	m_isTalking = false;
	m_isFinished = true;
	m_isPageFull = false;

	OnFinish();

}

void PNJ::SkipTalk()
{
	Text* dialogText = m_scene->GetGeometry<Text>("dialog");

	for (; m_index < m_initial.size(); m_index++)
	{
		std::string testText = m_toSay + m_initial[m_index];
		dialogText->SetText(testText);
		dialogText->SetPosition({ m_textX, m_textY }, { 0,0 });

		if (dialogText->GetDimension().GetY() > m_maxTextHeight)
		{
			dialogText->SetText(m_toSay);
			dialogText->SetPosition({ m_textX, m_textY }, { 0,0 });
			m_isPageFull = true;
			return; 
		}

		m_toSay = testText;
	}

	m_isFinished = true;
}

void PNJ::OnFinish()
{
	if (m_onFinish)
		m_onFinish();
}

void PNJ::UpdateDialog()
{
	if (m_timer > 0.f)
		return;
	m_timer = m_interval;
	
	if (m_isPageFull)
		return; 

	if (m_index >= m_initial.size())
	{
		m_isFinished = true;
		return;
	}

	
	std::string testText = m_toSay + m_initial[m_index];

	Text* dialogText = m_scene->GetGeometry<Text>("dialog");
	dialogText->SetText(testText);
	dialogText->SetPosition({ m_textX, m_textY }, { 0,0 });

	if (dialogText->GetDimension().GetY() > m_maxTextHeight)
	{
		dialogText->SetText(m_toSay);
		dialogText->SetPosition({ m_textX, m_textY }, { 0,0 });
		m_isPageFull = true;
		return; 
	}

	m_toSay = testText;
	m_index++;
}




