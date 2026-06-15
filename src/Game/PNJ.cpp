#include "pch.h"
#include "PNJ.h"
#include "2DPendentLib/InputManager.h"
#include <chrono>
#include <condition_variable>
#include <thread>
#include "DialogueManager.h"
#include "Dialogue.h"
#include <sstream>


static int LENGHT = 94;

void PNJ::OnInitialize()
{
	GetScene()->CreateFillRect("dialogBox", Vector2f(GetScene()->GetWindowWidth() / 2, GetScene()->GetWindowHeight() - 100), Vector2i(GetScene()->GetWindowWidth() / 1.9f, 100), Color(255, 255, 255, 255), true);
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);
	Vector2f dialogBoxPos = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition({ 0,0 });
	Vector2f pressSpacePos = GetScene()->GetGeometry<Rectangle>("dialogBox")->GetPosition({ 1,1 });

	float m_rectX = dialogBoxPos.GetX();
	float m_rectY = dialogBoxPos.GetY();

	m_textX = m_rectX + 10;
	m_textY = m_rectY + 10;

	GetScene()->CreateText("dialog1", Vector2f(m_textX, m_textY), "", Color(0, 0, 0, 255), true);
	GetScene()->GetGeometry<Text>("dialog1")->SetFontSize(19);
	GetScene()->GetGeometry<Text>("dialog1")->SetIsVisible(false);

	GetScene()->CreateText("dialog2", Vector2f(m_textX, m_textY + 20), "", Color(0, 0, 0, 255), true);
	GetScene()->GetGeometry<Text>("dialog2")->SetFontSize(19);
	GetScene()->GetGeometry<Text>("dialog2")->SetIsVisible(false);

	GetScene()->CreateText("dialog3", Vector2f(m_textX, m_textY + 40), "", Color(0, 0, 0, 255), true);
	GetScene()->GetGeometry<Text>("dialog3")->SetFontSize(19);
	GetScene()->GetGeometry<Text>("dialog3")->SetIsVisible(false);

	GetScene()->CreateText("dialog4", Vector2f(m_textX, m_textY + 60), "", Color(0, 0, 0, 255), true);
	GetScene()->GetGeometry<Text>("dialog4")->SetFontSize(19);
	GetScene()->GetGeometry<Text>("dialog4")->SetIsVisible(false);

	GetScene()->CreateText("button", Vector2f(m_textX + 100, m_textY + 50), "", Color(255,255, 255, 255));
	GetScene()->GetGeometry<Text>("button")->SetFontSize(19);
	GetScene()->GetGeometry<Text>("button")->SetIsVisible(false);
	
	GetScene()->CreateText("pressSpace", Vector2f(pressSpacePos.GetX()- 10, pressSpacePos.GetY() - 10), "Appuyer sur ESPACE pour continuer", Color(0, 0, 0, 230), true);
	GetScene()->GetGeometry<Text>("pressSpace")->SetFontSize(17);
	GetScene()->GetGeometry<Text>("pressSpace")->SetPosition({ pressSpacePos.GetX() - 10, pressSpacePos.GetY()  }, {1,1});
	GetScene()->GetGeometry<Text>("pressSpace")->SetIsVisible(false);

}

void PNJ::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_SPACE) && m_isInPlayerZone && m_isFinished) {
		EndTalk();
	}
	if (InputManager::Get()->IsKeyboardDown(K_SPACE) && m_index < m_initial.size())
	{
		for (int i = 0; i < m_lines.size(); i++)
		{
			GetScene()->GetGeometry<Text>("dialog" + std::to_string(i + 1))->SetText(m_lines[i]);
			GetScene()->GetGeometry<Text>("dialog" + std::to_string(i + 1))->SetPosition(Vector2f(m_textX, m_textY + (i * 20)), { 0,0.5f });
			m_isFinished = true;
		}
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
	m_lines = SplitDialogue(m_initial, LENGHT);

	m_index = 0;

	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog1")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog2")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog3")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog4")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("pressSpace")->SetIsVisible(true);
}

void PNJ::Talk(int p_dialogID, int p_choiceID)
{
	m_isTalking = true;
	m_isFinished = false;

	m_choices = DialogueManager::Get()->GetChoicesById(m_dialogPath, p_choiceID);
	m_initial = *DialogueManager::Get()->GetDialogueById(m_dialogPath, p_dialogID);
	m_lines = SplitDialogue(m_initial, LENGHT);

	m_index = 0;

	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog1")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog2")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog3")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("dialog4")->SetIsVisible(true);
	GetScene()->GetGeometry<Text>("pressSpace")->SetIsVisible(true);

}

void PNJ::EndTalk()
{
	GetScene()->GetGeometry<Rectangle>("dialogBox")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("dialog1")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("dialog2")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("dialog3")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("dialog4")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("button")->SetIsVisible(false);
	GetScene()->GetGeometry<Text>("pressSpace")->SetIsVisible(false);

	

	m_isTalking = false;
	m_isFinished = true;

}

void PNJ::UpdateDialog()
{
	m_timer -= GetDeltaTime();

	if (m_timer > 0.f)
		return;

	if (m_index >= m_initial.size())
	{
		m_isFinished = true;
		if (m_choices.size() > 0)
		{
			int i = 1;
			for (Dialogue::Choice choice : m_choices)
			{
				std::string id = choice.s_label + std::to_string(i++);
				m_button = GetScene()->CreateButton({ 250.f * i, 200 }, { 50, 25 }, "", id, choice.s_label, true);
				m_button->SetOnClick([this, id, choice] {
					Talk(choice.s_target, choice.s_target);
					m_button->SetIsVisible(false);
					});
			}
		}
		return;
	}

	m_index++;

	size_t displayedChars = m_index;
	size_t accumulatedChars = 0;

	// Ligne 1
	if (m_lines.size() > 0)
	{
		size_t charsForThisLine =
			std::min(displayedChars, m_lines[0].size());

		GetScene()->GetGeometry<Text>("dialog1")
			->SetText(m_lines[0].substr(0, charsForThisLine));

		accumulatedChars += m_lines[0].size();
	}

	// Ligne 2
	if (m_lines.size() > 1 && displayedChars > accumulatedChars)
	{
		size_t charsForThisLine =
			std::min(displayedChars - accumulatedChars,
				m_lines[1].size());

		GetScene()->GetGeometry<Text>("dialog2")
			->SetText(m_lines[1].substr(0, charsForThisLine));

		accumulatedChars += m_lines[1].size();
	}

	// Ligne 3
	if (m_lines.size() > 2 && displayedChars > accumulatedChars)
	{
		size_t charsForThisLine =
			std::min(displayedChars - accumulatedChars,
				m_lines[2].size());

		GetScene()->GetGeometry<Text>("dialog3")
			->SetText(m_lines[2].substr(0, charsForThisLine));

		accumulatedChars += m_lines[2].size();
	}

	// Ligne 4
	if (m_lines.size() > 3 && displayedChars > accumulatedChars)
	{
		size_t charsForThisLine =
			std::min(displayedChars - accumulatedChars,
				m_lines[3].size());

		GetScene()->GetGeometry<Text>("dialog4")
			->SetText(m_lines[3].substr(0, charsForThisLine));
	}

	m_timer = m_interval;

	GetScene()->GetGeometry<Text>("dialog1")->SetPosition(
		Vector2f(m_textX, m_textY),
		{ 0, 0.5f });

	GetScene()->GetGeometry<Text>("dialog2")->SetPosition(
		Vector2f(m_textX, m_textY + 20),
		{ 0, 0.5f });

	GetScene()->GetGeometry<Text>("dialog3")->SetPosition(
		Vector2f(m_textX, m_textY + 40),
		{ 0, 0.5f });

	GetScene()->GetGeometry<Text>("dialog4")->SetPosition(
		Vector2f(m_textX, m_textY + 60),
		{ 0, 0.5f });
}

std::vector<std::string> PNJ::SplitDialogue(const std::string& text, size_t maxLength)
{
	std::vector<std::string> lines;

	std::stringstream ss(text);
	std::string word;

	std::string currentLine;

	while (ss >> word)
	{
		if (currentLine.empty())
		{
			currentLine = word;
		}
		else if (currentLine.size() + word.size() + 1 <= maxLength)
		{
			currentLine += " " + word;
		}
		else
		{
			lines.push_back(currentLine);
			currentLine = word;
		}
	}

	if (!currentLine.empty())
		lines.push_back(currentLine);

	return lines;
}


