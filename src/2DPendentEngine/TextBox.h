#pragma once
#include <SDL3/SDL.h>
#include "Button.h"
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/Color.h"
#include "2DPendentLib/SDLEncapsulation.h"
#include "2DPendentLib/Text.h"

class Window;
class Render;
class Camera;

class TextBox : public Button
{
private:

	Text* m_textToDraw = nullptr;

	Window* m_window = nullptr;
	void SetWindow(Window* p_window) { m_window = p_window; }

	SDLEncapsulation::S_Rect* m_BoxText = nullptr;

	Vector2f m_textOffset = { 18.f, 0.0f };

public:

	TextBox();
	~TextBox();

	void InitTextBox(const std::string& p_id, Color p_color = ColorsNamed::Yellow, bool p_isGUI = true);

	void SetText(const std::string& p_text);
	const std::string& GetText() const { return m_textToDraw->GetText(); }

	void OnDraw(Render* f_render, Window* f_window) override;


	friend class Scene;

};

