#include "pch.h"
#include "Button.h"
#include "Debug.h"
#include "InputManager.h"
#include "Application.h"

void Button::InitText(const std::string& p_textID, const std::string& p_text)
{
	m_textID = p_textID;
	Debug::Get()->CreateText(p_textID, GetPosition(), p_text, { 255, 255, 255, 255 }, GetIsGUI());
}

void Button::SetText(const std::string& p_text)
{
	if (Debug::Get()->GetText(m_textID) == nullptr)
		return;

	Debug::Get()->GetText(m_textID)->SetText(p_text);
}

void Button::SetTextOffset(const Vector2f& p_offset)
{
	Debug::Get()->GetText(m_textID)->SetPosition(GetPosition() + p_offset);
}

bool Button::IsMouseIn()
{
	InputManager* im = InputManager::Get();
	Vector2f mousePos = im->GetMousePosition();
	Vector2f pos = Application::Get()->GetCamera()->WorldToScreen(GetPosition({0.f, 0.f}));
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

	// LEFT CLICK
	if (mouseIn && im->IsMouseUp(BUTTON_LEFT))
	{
		if (m_onReleaseLeftClick)
			m_onReleaseLeftClick();
	}
	else if (mouseIn && im->IsMouseDown(BUTTON_LEFT))
	{
		if (m_onLeftClick)
			m_onLeftClick();
	}
	else if (mouseIn && im->IsMouseHeld(BUTTON_LEFT))
	{
		if (m_onHeldLeftClick)
			m_onHeldLeftClick();
	}

	// RIGHT CLICK
	if (mouseIn && im->IsMouseUp(BUTTON_RIGHT))
	{
		if (m_onReleaseRightClick)
			m_onReleaseRightClick();
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
