#include "pch.h"
#include "Button.h"
#include "Debug.h"
#include "InputManager.h"
#include "Application.h"

Button::~Button()
{
	GetScene()->RemoveGeometry(m_textID);
}

void Button::InitText(const std::string& p_textID, const std::string& p_text)
{
	m_textID = p_textID;
	GetScene()->CreateText(p_textID, GetPosition(), p_text, { 255, 255, 255, 255 }, GetIsGUI());
}

void Button::SetText(const std::string& p_text)
{
	GetScene()->GetGeometry<Text>(m_textID)->SetText(p_text);
}

void Button::SetTextOffset(const Vector2f& p_offset)
{
	GetScene()->GetGeometry<Text>(m_textID)->SetPosition(GetPosition() + p_offset);
}

bool Button::IsMouseIn()
{
	InputManager* im = InputManager::Get();
	Vector2f mousePos = im->GetMousePosition();
	Vector2f pos = Application::Get()->GetCamera()->WorldToScreen(GetCollider()->GetPosition({ 0.f, 0.f }));
	//std::cout << pos.GetX() << " " << pos.GetY() << std::endl;
	//std::cout << mousePos.GetX() << " " << mousePos.GetY() << std::endl;
	int w = GetCollider()->GetDimension().GetX();
	int h = GetCollider()->GetDimension().GetY();

	if (mousePos.GetX() < pos.GetX() || mousePos.GetY() < pos.GetY())
		return false;
	else if (mousePos.GetX() > pos.GetX() + w || mousePos.GetY() > pos.GetY() + h)
		return false;

	return true;
}

void Button::OnUpdate()
{
	InputManager* im = InputManager::Get();
	bool mouseIn = IsMouseIn();

	// HOVER
	if (mouseIn && m_mouseWasIn == false)
	{
		if (m_onHoverEnter)
			m_onHoverEnter();
	}
	else if (mouseIn)
	{
		if (m_onHoverStay)
			m_onHoverStay();
	}
	else if (m_mouseWasIn && mouseIn == false)
	{
		if (m_onHoverExit)
			m_onHoverExit();
	}

	auto now = std::chrono::steady_clock::now();

	// LEFT CLICK
	if (mouseIn && im->IsMouseUp(BUTTON_LEFT))
	{
		if (m_onReleaseLeftClick)
			m_onReleaseLeftClick();

		/*float elapsed = std::chrono::duration<float>(now - m_lastLeftClick).count();

		if (elapsed <= m_doubleClickDelay)
		{
			if (m_onDoubleLeftClick)
				m_onDoubleLeftClick();
		}

		m_lastLeftClick = now;*/


		// Si tu veux que quand ya un double click, le click simple ne se fasse pas, tu peux faire ca :

		

		float elapsed = std::chrono::duration<float>(now - m_lastLeftClick).count();

		if (elapsed <= m_doubleClickDelay)
		{
			if (m_onDoubleLeftClick)
				m_onDoubleLeftClick();
		}
		else
		{
			if (m_onReleaseLeftClick)
				m_onReleaseLeftClick();
		}

		m_lastLeftClick = now;

		
	}
	else if (mouseIn && im->IsMouseHeld(BUTTON_LEFT))
	{
		if (m_onHeldLeftClick)
			m_onHeldLeftClick();
	}
	else if (mouseIn && im->IsMouseDown(BUTTON_LEFT))
	{
		if (m_onLeftClick)
			m_onLeftClick();
	}

	// RIGHT CLICK
	if (mouseIn && im->IsMouseUp(BUTTON_RIGHT))
	{
		if (m_onReleaseRightClick)
			m_onReleaseRightClick();

		float elapsed = std::chrono::duration<float>(now - m_lastRightClick).count();

		if (elapsed <= m_doubleClickDelay)
		{
			if (m_onDoubleRightClick)
				m_onDoubleRightClick();
		}

		m_lastRightClick = now;
	}
	else if (mouseIn && im->IsMouseDown(BUTTON_RIGHT))
	{
		if (m_onRightClick)
			m_onRightClick();
	}
	else if (mouseIn && im->IsMouseHeld(BUTTON_RIGHT))
	{
		if (m_onHeldRightClick)
			m_onHeldRightClick();
	}

	m_mouseWasIn = mouseIn;
}