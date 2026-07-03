#include "pch.h"
#include "TextBox.h"
#include "Debug.h"
#include "Window.h"

TextBox::TextBox()
{
	m_textToDraw = new Text();

}

TextBox::~TextBox()
{


}

void TextBox::InitTextBox(const std::string& p_id, Color p_color, bool p_isGUI)
{
	Vector2f position = GetPosition();
	Vector2i dimension = GetDimension();
	Debug::Get()->CreateRect(p_id, position, dimension, p_color, p_isGUI, false);

	m_BoxText = new SDLEncapsulation::S_Rect();

	m_BoxText->x = static_cast<int>(position.GetX());
	m_BoxText->y = static_cast<int>(position.GetY());
	m_BoxText->w = dimension.GetX();
	m_BoxText->h = dimension.GetY();

	SDLEncapsulation::SetTextInputArea(m_window, m_BoxText, 0);
	m_textToDraw->SetRenderer(Application::Get()->GetRender()->GetRenderer());
	m_textToDraw->Initialize(GetPosition({0.0f, 0.5f}), "", ColorsNamed::Red);
	m_textToDraw->SetIsGUI(p_isGUI);
}

void TextBox::SetText(const std::string& p_text)
{
	m_textToDraw->SetText(p_text);
}

void TextBox::OnDraw(Render* f_render, Window* f_window)
{
	m_textToDraw->SetPosition(GetPosition({ 0.0f, 0.5f }) + m_textOffset, {0.0f, 0.5f});
	//if (m_textToDraw->GetText().empty == true)
	//{
	//	/*m_textToDraw->SetText("...");
	//	m_textToDraw->SetColor(ColorsNamed::Gray);*/
	//	m_textToDraw->SetText(" ");
	//}
	//else
	//{
	//	/*m_textToDraw->SetColor(ColorsNamed::Red);*/
	//}
	m_textToDraw->Draw(f_render, f_window, Application::Get()->GetCamera());
}
