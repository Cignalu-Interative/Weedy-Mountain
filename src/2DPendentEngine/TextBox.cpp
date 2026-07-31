#include "pch.h"
#include "TextBox.h"
#include "Debug.h"
#include "Window.h"
#include "SpriteContainer.h"

TextBox::TextBox()
{
	m_textToDraw = new Text();
	m_rectangle = new Rectangle();
	
}

TextBox::~TextBox()
{


}

void TextBox::InitTextBox(const std::string& p_id, Color p_color, bool p_isGUI)
{
	AddSprite("textbox", "", GetDimension(), 15);
	//RemoveSprite("default");
	Vector2f position = GetPosition();
	Vector2i dimension = GetDimension();
	m_rectangle->Initialize(position, dimension, p_color);
	m_rectangle->SetIsGUI(GetIsGUI());
	m_rectangle->SetDrawLayer(INT_MAX - 100);

	m_BoxText = new SDLEncapsulation::S_Rect();

	m_BoxText->x = static_cast<int>(position.GetX());
	m_BoxText->y = static_cast<int>(position.GetY());
	m_BoxText->w = dimension.GetX();
	m_BoxText->h = dimension.GetY();

	SDLEncapsulation::SetTextInputArea(m_window, m_BoxText, 0);
	m_textToDraw->SetRenderer(Application::Get()->GetRender()->GetRenderer());
	m_textToDraw->SetTextEngine(Debug::Get()->GetTextEngine());
	m_textToDraw->Initialize(GetPosition({0.0f, 0.5f}), "", ColorsNamed::LightPink);
	m_textToDraw->SetIsGUI(GetIsGUI());
	m_textToDraw->SetIsGUI(p_isGUI);
	m_textToDraw->SetDrawLayer(INT_MAX);
	m_textToDraw->SetWrapWidth(dimension.GetX() - m_textOffset.GetX());
	SetIsVisible(true);
}

void TextBox::SetTextInBox(const std::string& p_text)
{
	m_textToDraw->SetText(p_text);
	m_textToDraw->CutTextBySize(GetDimension() - (Vector2i)m_textOffset);
}

void TextBox::OnDraw(Render* f_render, Window* f_window)
{
	if (m_isVisible == false)
	{
		return;
	}

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
	m_rectangle->Draw(f_render, f_window, Application::Get()->GetCamera());
	m_textToDraw->Draw(f_render, f_window, Application::Get()->GetCamera());
}
