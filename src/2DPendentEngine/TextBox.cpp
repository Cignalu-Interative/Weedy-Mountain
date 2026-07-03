#include "pch.h"
#include "TextBox.h"
#include "Debug.h"
#include "Window.h"

void TextBox::InitTextBox(const std::string& p_id, Color p_color, bool p_isGUI)
{
	Vector2f position = GetPosition();
	Vector2i dimension = GetDimension();
	Debug::Get()->CreateRect(p_id, position, dimension, p_color, p_isGUI, false);

	m_BoxText->x = static_cast<int>(position.GetX());
	m_BoxText->y = static_cast<int>(position.GetY());
	m_BoxText->w = dimension.GetX();
	m_BoxText->h = dimension.GetY();

	SDLEncapsulation::SetTextInputArea(m_window, m_BoxText, 0);
}
