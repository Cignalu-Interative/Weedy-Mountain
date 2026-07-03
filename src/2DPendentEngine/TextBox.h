#pragma once
#include <SDL3/SDL.h>
#include "Button.h"
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/Color.h"
#include "SDLEncapsulation.h"

class Window;
class TextBox : public Button
{
private:

	Window* m_window = nullptr;
	void SetWindow(Window* p_window) { m_window = p_window; }

	SDLEncapsulation::S_Rect* m_BoxText = nullptr;

public:

	void InitTextBox(const std::string& p_id, Color p_color = ColorsNamed::Yellow, bool p_isGUI = true);


	friend class Scene;

};

