#include "pch.h"
#include "Button.h"
#include "Debug.h"
#include "InputManager.h"

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

bool Button::IsMouseIn()
{
	InputManager* im = InputManager::Get();
	Vector2f mousePos = im->GetMousePosition();
	Vector2f pos = GetPosition({0.f, 0.f});
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

	// CLICK
	if (mouseIn && im->IsMouseUp(BUTTON_LEFT))
	{
		if (m_onReleaseClick)
			m_onReleaseClick();
	}
	else if (mouseIn && im->IsMouseDown(BUTTON_LEFT))
	{
		if (m_onClick)
			m_onClick();
	}
	else if (mouseIn && im->IsMouseHeld(BUTTON_LEFT))
	{
		if (m_onHeldClick)
			m_onHeldClick();
	}

	m_mouseWasIn = mouseIn;
}
