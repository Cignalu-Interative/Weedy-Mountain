#include "pch.h"
#include "Button.h"
#include "Debug.h"
#include "InputManager.h"
#include "Application.h"

Button::~Button()
{
	m_text = nullptr;
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
	Vector2f pos = GetPosition({ 0,0 });
	//Vector2f pos = Application::Get()->GetCamera()->WorldToScreen(GetCollider()->GetPosition({0.f, 0.f}));
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
	if(m_isVisible == false)
		return;

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

	// HELD ENTER / HELD EXIT
	if (mouseIn && m_mouseWasIn == false)
	{
		if (im->IsMouseHeld(BUTTON_LEFT))
		{
			if (m_onLeftHeldEnter)
				m_onLeftHeldEnter();
		}

		if (im->IsMouseHeld(BUTTON_RIGHT))
		{
			if (m_onRightHeldEnter)
				m_onRightHeldEnter();
		}
	}
	else if (mouseIn == false && m_mouseWasIn)
	{
		if (im->IsMouseHeld(BUTTON_RIGHT))
		{
			if (m_onRightHeldExit)
				m_onRightHeldExit();
		}
		if (im->IsMouseHeld(BUTTON_LEFT))
		{
			if (m_onLeftHeldExit)
				m_onLeftHeldExit();
		}
	}




	auto now = std::chrono::steady_clock::now();

	// LEFT CLICK
	if (mouseIn && im->IsMouseUp(BUTTON_LEFT))
	{
		if (m_onReleaseLeftClick)
			m_onReleaseLeftClick();

		m_leftWasDown = false;
	}
	else if (mouseIn && im->IsMouseHeld(BUTTON_LEFT))
	{
		if (m_leftWasDown)
		{
			if (m_onHeldLeftClick)
				m_onHeldLeftClick();
		}
	}
	else if (mouseIn && im->IsMouseDown(BUTTON_LEFT))
	{
		float elapsed = std::chrono::duration<float>(now - m_lastLeftClick).count();

		if (elapsed <= m_doubleClickDelay)
		{
			if (m_onDoubleLeftClick)
				m_onDoubleLeftClick();
		}
		else
		{
			if (m_onLeftClick)
				m_onLeftClick();
		}

		m_lastLeftClick = now;
		m_leftWasDown = true;
	}

	// RIGHT CLICK
	if (mouseIn && im->IsMouseUp(BUTTON_RIGHT))
	{
		if (m_onReleaseRightClick)
			m_onReleaseRightClick();

		m_rightWasDown = false;
	}
	else if (mouseIn && im->IsMouseHeld(BUTTON_RIGHT))
	{
		if (m_rightWasDown)
		{
			if (m_onHeldRightClick)
				m_onHeldRightClick();
		}
	}
	else if (mouseIn && im->IsMouseDown(BUTTON_RIGHT))
	{
		float elapsed = std::chrono::duration<float>(now - m_lastRightClick).count();

		if (elapsed <= m_doubleClickDelay)
		{
			if (m_onDoubleRightClick)
				m_onDoubleRightClick();
		}
		else
		{
			if (m_onRightClick)
				m_onRightClick();
		}

		m_rightWasDown = true;
		m_lastRightClick = now;
	}

	//Middke click
	if (mouseIn && im->IsMouseDown(BUTTON_MIDDLE))
	{
		if (m_onMiddleClick)
			m_onMiddleClick();
	}

	m_mouseWasIn = mouseIn;

	//std::cout << "Button pos: " << GetPosition().GetX() << " " << GetPosition().GetY() << std::endl;
	//std::cout << "Text pos: " << GetScene()->GetGeometry<Text>(m_textID)->GetPosition().GetX() << " " << GetScene()->GetGeometry<Text>(m_textID)->GetPosition().GetY() << std::endl;
}

void Button::SetIsVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);

	if (GetScene()->GetGeometry<Text>(m_textID) == nullptr)
		return;

	GetScene()->GetGeometry<Text>(m_textID)->SetIsVisible(p_isVisible);

}
