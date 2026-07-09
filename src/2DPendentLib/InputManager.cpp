#include "pch.h"
#include "InputManager.h"
#include "Debug.h"
#include "AssetManager.h"
#include "Window.h"
#include "Utils.h"

InputManager* InputManager::Get()
{
	static InputManager instance;
	return &instance;
}

void InputManager::Update()
{
	for (int i = 0; i < NUM_SCANCODES; i++)
	{
		if (m_keyboardStates[i].s_isDown)
		{
			m_keyboardStates[i].s_isHeld = true;
		}
		if (m_keyboardStates[i].s_isUp)
		{
			m_keyboardStates[i].s_isHeld = false;
		}
		m_keyboardStates[i].s_isDown = false;
		m_keyboardStates[i].s_isUp = false;
	}
	for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
	{
		if (m_mouseStates[i].s_isDown)
		{
			m_mouseStates[i].s_isHeld = true;
		}
		if (m_mouseStates[i].s_isUp)
		{
			m_mouseStates[i].s_isHeld = false;
		}
		m_mouseStates[i].s_isDown = false;
		m_mouseStates[i].s_isUp = false;
	}
	for (int i = 0; i < SCROLL_MAX; i++) {
		if (m_mouseScrollStates[i].s_isDown) {
			m_mouseScrollStates[i].s_isHeld = true;
		}
		else {
			m_mouseScrollStates[i].s_isHeld = false; 
		}
		m_mouseScrollStates[i].s_isDown = false; 
	}
	m_mouseWheelX = 0;
	m_mouseWheelY = 0;
	for (int i = 0; i < CONTROLLER_BUTTON_MAX; i++)
	{
		if (m_gameControllerButtonStates[i].s_isDown)
		{
			m_gameControllerButtonStates[i].s_isHeld = true;
		}
		if (m_gameControllerButtonStates[i].s_isUp)
		{
			m_gameControllerButtonStates[i].s_isHeld = false;
		}
		m_gameControllerButtonStates[i].s_isDown = false;
		m_gameControllerButtonStates[i].s_isUp = false;
	}
	for (int i = 0; i < CONTROLLER_AXIS_MAX; i++)
	{
		if (m_gameControllerAxisStates[i].s_isDown)
		{
			m_gameControllerAxisStates[i].s_isHeld = true;
		}
		if (m_gameControllerAxisStates[i].s_isUp)
		{
			m_gameControllerAxisStates[i].s_isHeld = false;
		}
		m_gameControllerAxisStates[i].s_isDown = false;
		m_gameControllerAxisStates[i].s_isUp = false;
	}

	ClearMouseRelative();

	if (m_isTextInput == false)
	{
		m_compositionTextInput.clear();
		m_finalTextInput.clear();
	}

	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
		case SDL_EVENT_QUIT:
			if (m_quitCallback)
			{
				m_quitCallback();
			}
			break;

		case SDL_EVENT_KEY_DOWN:
			if (m_event.key.repeat)
			{
				break;
			}
			m_keyboardStates[m_event.key.scancode].s_isDown = true;
			break;

		case SDL_EVENT_KEY_UP:
			if (m_event.key.repeat)
			{
				break;
			}
			m_keyboardStates[m_event.key.scancode].s_isUp = true;
			break;

		case SDL_EVENT_TEXT_EDITING:
			//std::cerr << "Texte en cours d'édition : " << m_event.edit.text << std::endl;
			m_compositionTextInput = m_event.edit.text;
			m_cursorPositionX = m_event.edit.start;
			m_cursorLength = m_event.edit.length;
			break;

		case SDL_EVENT_TEXT_INPUT:
			//std::cerr << "Texte saisi : " << m_event.text.text << std::endl;
			m_finalTextInput += m_event.text.text;
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			m_mouseStates[m_event.button.button].s_isDown = true;
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			m_mouseStates[m_event.button.button].s_isUp = true;
			break;

		case SDL_EVENT_MOUSE_MOTION:
			m_ABSMousePosition.SetX(m_event.motion.x);   // Position X absolue dans la fenêtre
			m_ABSMousePosition.SetY(m_event.motion.y);   // Position Y absolue dans la fenêtre
			m_RELMousePosition.SetX(m_event.motion.xrel); // Déplacement relatif sur l'axe X
			m_RELMousePosition.SetY(m_event.motion.yrel); // Déplacement relatif sur l'axe Y
			break;

		case SDL_EVENT_MOUSE_WHEEL:
			if (m_event.wheel.x > 0)
			{
				m_mouseScrollStates[SCROLL_RIGHT].s_isDown = true;
			}
			else if (m_event.wheel.x < 0)
			{
				m_mouseScrollStates[SCROLL_LEFT].s_isDown = true;
			}

			if (m_event.wheel.y > 0)
			{
				m_mouseScrollStates[SCROLL_UP].s_isDown = true;
			}
			else if (m_event.wheel.y < 0)
			{
				m_mouseScrollStates[SCROLL_DOWN].s_isDown = true;
			}
			break;

		case SDL_EVENT_GAMEPAD_ADDED:
			OpenGameController();
			break;

		case SDL_EVENT_GAMEPAD_REMOVED:
			if (m_gameController && SDL_GetGamepadID(m_gameController) == m_event.gdevice.which) {
				SDL_CloseGamepad(m_gameController);
				m_gameController = nullptr;
				std::cerr << "Manette deconnectee\n";
			}
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			m_gameControllerButtonStates[m_event.gbutton.button].s_isDown = true;
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			m_gameControllerButtonStates[m_event.gbutton.button].s_isUp = true;
			break;

		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			int axis = m_event.gaxis.axis;
			Sint16 rawValue = m_event.gaxis.value;

			//constexpr float INV_32768 = 1.0f / 32768.0f;
			//constexpr float INV_32767 = 1.0f / 32767.0f;

			//float normalized = rawValue * ((rawValue < 0) ? INV_32768 : INV_32767);

			float normalized = (rawValue < 0) ? (rawValue / 32768.0f) : (rawValue / 32767.0f);

			if (std::fabs(normalized) < AXIS_DEAD_ZONE)
			{
    			m_gameControllerAxisStatesFloat[axis] = 0.0f;
				m_gameControllerAxisStates[axis].s_isUp = true;
			}
			else
			{
				float value = (normalized > 0) ? (normalized - AXIS_DEAD_ZONE) / (1.0f - AXIS_DEAD_ZONE) : (normalized + AXIS_DEAD_ZONE) / (1.0f - AXIS_DEAD_ZONE);
				m_gameControllerAxisStatesFloat[axis] = value;
				m_gameControllerAxisStates[axis].s_isDown = true;
			}
			break;
		}
	}
}

bool InputManager::IsKeyboardDown(KeyboardKeycodeKeys p_key) const
{
	KeyboardScancodeKeys scancode = static_cast<KeyboardScancodeKeys>(SDL_GetScancodeFromKey(p_key, nullptr));
	return m_keyboardStates[scancode].s_isDown;
}
bool InputManager::IsKeyboardHeld(KeyboardKeycodeKeys p_key) const
{
	KeyboardScancodeKeys scancode = static_cast<KeyboardScancodeKeys>(SDL_GetScancodeFromKey(p_key, nullptr));
	return m_keyboardStates[scancode].s_isHeld;
}
bool InputManager::IsKeyboardUP(KeyboardKeycodeKeys p_key) const
{
	KeyboardScancodeKeys scancode = static_cast<KeyboardScancodeKeys>(SDL_GetScancodeFromKey(p_key, nullptr));
	return m_keyboardStates[scancode].s_isUp;
}

bool InputManager::IsMouseDown(Uint8 p_button) const
{
	return p_button < MOUSE_BUTTON_MAX && m_mouseStates[p_button].s_isDown;
}
bool InputManager::IsMouseHeld(Uint8 p_button) const
{
	return p_button < MOUSE_BUTTON_MAX && m_mouseStates[p_button].s_isHeld;
}
bool InputManager::IsMouseUp(Uint8 p_button) const
{
	return p_button < MOUSE_BUTTON_MAX && m_mouseStates[p_button].s_isUp;
}

bool InputManager::IsMouseScrollDown(MouseScroll p_scroll) const
{
	return m_mouseScrollStates[p_scroll].s_isDown;
}
bool InputManager::IsMouseScrollHeld(MouseScroll p_scroll) const
{
	return m_mouseScrollStates[p_scroll].s_isHeld;
}
bool InputManager::IsMouseScrollUp(MouseScroll p_scroll) const
{
	return m_mouseScrollStates[p_scroll].s_isUp;
}

bool InputManager::IsGameControllerButtonDown(ControllerButton p_button) const
{
	return m_gameControllerButtonStates[p_button].s_isDown;
}
bool InputManager::IsGameControllerButtonHeld(ControllerButton p_button) const
{
	return m_gameControllerButtonStates[p_button].s_isHeld;
}
bool InputManager::IsGameControllerButtonUp(ControllerButton p_button) const
{
	return m_gameControllerButtonStates[p_button].s_isUp;
}

bool InputManager::IsGameControllerAxisDown(ControllerAxis p_axis) const
{
	return m_gameControllerAxisStates[p_axis].s_isDown;
}
bool InputManager::IsGameControllerAxisHeld(ControllerAxis p_axis) const
{
	return m_gameControllerAxisStates[p_axis].s_isHeld;
}
bool InputManager::IsGameControllerAxisUp(ControllerAxis p_axis) const
{
	return m_gameControllerAxisStates[p_axis].s_isUp;
}

bool InputManager::IsJoyStickDown(JoyStick p_joystick) const
{
	switch (p_joystick) {
	case JOYSTICK_LEFT:
		// Left stick down = LEFTX OU LEFTY down
		return IsGameControllerAxisDown(CONTROLLER_AXIS_LEFTX) ||
			IsGameControllerAxisDown(CONTROLLER_AXIS_LEFTY);

	case JOYSTICK_RIGHT:
		return IsGameControllerAxisDown(CONTROLLER_AXIS_RIGHTX) ||
			IsGameControllerAxisDown(CONTROLLER_AXIS_RIGHTY);

	default: return false;
	}
}
bool InputManager::IsJoyStickHeld(JoyStick p_joystick) const
{
	switch (p_joystick) {
	case JOYSTICK_LEFT:
		// Left stick held = LEFTX OU LEFTY held
		return IsGameControllerAxisHeld(CONTROLLER_AXIS_LEFTX) ||
			IsGameControllerAxisHeld(CONTROLLER_AXIS_LEFTY);

	case JOYSTICK_RIGHT:
		return IsGameControllerAxisHeld(CONTROLLER_AXIS_RIGHTX) ||
			IsGameControllerAxisHeld(CONTROLLER_AXIS_RIGHTY);

	default: return false;
	}
}
bool InputManager::IsJoyStickUp(JoyStick p_joystick) const
{
	switch (p_joystick) {
	case JOYSTICK_LEFT:
		// Left stick up = LEFTX ET LEFTY up
		return IsGameControllerAxisUp(CONTROLLER_AXIS_LEFTX) &&
			IsGameControllerAxisUp(CONTROLLER_AXIS_LEFTY);

	case JOYSTICK_RIGHT:
		return IsGameControllerAxisUp(CONTROLLER_AXIS_RIGHTX) &&
			IsGameControllerAxisUp(CONTROLLER_AXIS_RIGHTY);

	default: return false;
	}
}

float InputManager::GetMouseX()
{
	float x, y;
	SDL_GetMouseState(&x, &y);
	return x;
}
float InputManager::GetMouseY()
{
	float x, y;
	SDL_GetMouseState(&x, &y);
	return y;
}

Vector2f InputManager::GetMousePosition()
{
	float x, y;
	SDL_GetMouseState(&x, &y);
	return Vector2f(x, y);
}

Vector2i InputManager::GetABSMousePosition()
{
	return m_ABSMousePosition;
}

Vector2i InputManager::GetRELMousePosition()
{
	return m_RELMousePosition;
}

int InputManager::GetMouseWheelX()
{
	return m_mouseWheelX;
}
int InputManager::GetMouseWheelY()
{
	return m_mouseWheelY;
}
Vector2i InputManager::GetMouseWheel()
{
	return Vector2i(m_mouseWheelX, m_mouseWheelY);
}

Vector2f InputManager::GetJoyStickOrientation(JoyStick p_joystick)
{
	Vector2f dir{ 0.0f, 0.0f };

	float x, y;

	switch (p_joystick) {
	case JOYSTICK_LEFT:
		x = m_gameControllerAxisStatesFloat[CONTROLLER_AXIS_LEFTX];
		y = m_gameControllerAxisStatesFloat[CONTROLLER_AXIS_LEFTY];
		break;
	case JOYSTICK_RIGHT:
		x = m_gameControllerAxisStatesFloat[CONTROLLER_AXIS_RIGHTX];
		y = m_gameControllerAxisStatesFloat[CONTROLLER_AXIS_RIGHTY];
		break;
	}

	// Deadzone + normalisation
	float len = sqrtf(x * x + y * y);

	if (len > AXIS_DEAD_ZONE)
	{
		float normalizedLen = (len - AXIS_DEAD_ZONE) / (1.0f - AXIS_DEAD_ZONE);

		float limitedLen = Utils::LimitMax(normalizedLen, 1.0f);

		float xFinal = (x / len) * limitedLen;
		float yFinal = (y / len) * limitedLen;
		dir.SetX(xFinal);
		dir.SetY(yFinal);
	}
	else {
		dir.SetX(0.0f);
		dir.SetY(0.0f);
	}
	return dir;
}

void InputManager::SetCursor(const std::string& path, Vector2f p_position)
{
	if (path == m_cursorPath)
	{
		return; // Le chemin du curseur n'a pas changé, pas besoin de recréer le curseur
	}
	m_cursorPath = path;
	SDL_Surface* surf = IMG_Load(m_cursorPath.c_str());
	if (surf == nullptr)
	{
		SDL_Log("Lib/InputManager.cpp/SetCursor/IMG_Load/IMG_Load non reussie : %s", SDL_GetError());
		return;
	}

	if (m_cursor != nullptr)
	{
		SDL_DestroyCursor(m_cursor);
		m_cursor = nullptr;
	}

	m_cursor = SDL_CreateColorCursor(surf, p_position.GetX(), p_position.GetY());

	SDL_DestroySurface(surf);

	if (m_cursor == NULL)
	{
		SDL_Log("Lib/InputManager.cpp/SetCursor/SDL_CreateColorCursor/SDL_CreateColorCursor non reussie : %s", SDL_GetError());
		return;
	}

	if (SDL_SetCursor(m_cursor) == false)
	{
		SDL_Log("Lib/InputManager.cpp/SetCursor/SDL_SetCursor/SDL_SetCursor non reussie : %s", SDL_GetError());
		return;
	}
}

#ifdef _DEBUG
void InputManager::DebugKeys()
{
	for (int i = 0; i < NUM_SCANCODES; i++)
	{
		if (m_keyboardStates[i].s_isDown)
		{
			switch (i)
			{
			default:
				SDL_Scancode scancode = static_cast<SDL_Scancode>(i);
				SDL_Keycode kc = SDL_GetKeyFromScancode(scancode, SDL_KMOD_NONE, true);
				const char* keyName = SDL_GetKeyName(kc);
				std::cerr << "Scancode: " << i
					<< " | Keycode Name:"
					<< " (" << (keyName ? keyName : "Unknown") << ") down"
					<< " | Value: " << kc
					<< std::endl;
			}
		}
	}
}
void InputManager::DebugControllerButtons()
{
	for (int i = 0; i < CONTROLLER_BUTTON_MAX; i++)
	{
		if (m_gameControllerButtonStates[i].s_isDown)
		{
			switch (i)
			{
			default:
				State& state = m_gameControllerButtonStates[i];
				std::cerr << "Controller Button ( " << i << " ) down" << std::endl;
			}
		}
	}
}
void InputManager::DebugControllerAxis()
{
	for (int i = 0; i < CONTROLLER_AXIS_MAX; i++)
	{
		if (m_gameControllerAxisStates[i].s_isDown)
		{
			switch (i)
			{
			default:
				State& state = m_gameControllerAxisStates[i];
				std::cerr << "Controller Axis ( " << i << " ) down" << std::endl;
			}
		}
	}
}
#endif

void InputManager::SetSDLWarpMouseInWindow(Vector2f p_position)
{
	SDL_WarpMouseInWindow(m_window->GetWindow(), (int)p_position.GetX(), (int)p_position.GetY());
}
void InputManager::SetRelativeMouseMode(bool p_bool)
{
	if (SDL_SetWindowRelativeMouseMode(m_window->GetWindow(), p_bool) == false)
	{
		SDL_Log("Lib/InputManager.cpp/SetRelativeMouseMode/SDL_SetWindowRelativeMouseMode non reussie : %s", SDL_GetError());
		return;
	}
}

void InputManager::ClearMouseRelative()
{
	m_RELMousePosition.SetX(0);
	m_RELMousePosition.SetY(0);
}

void InputManager::StartTextInput()
{
	m_isTextInput = true;
	if (SDL_StartTextInput(m_window->GetWindow()) == false)
	{
		SDL_Log("Lib/InputManager.cpp/SetStartTextInput/SDL_StartTextInput non reussie : %s", SDL_GetError());
		return;
	}
	std::cerr << "InputManager : SDL_StartTextInput" << std::endl;
}
void InputManager::StopTextInput()
{
	m_isTextInput = false;
	if (SDL_StopTextInput(m_window->GetWindow()) == false)
	{
		SDL_Log("Lib/InputManager.cpp/SetStartTextInput/SDL_StopTextInput non reussie : %s", SDL_GetError());
		return;
	}
	std::cerr << "InputManager : SDL_StopTextInput" << std::endl;
}

void InputManager::SetWindow(Window* p_window)
{
	m_window = p_window;
}

InputManager::InputManager()
{
	std::memset(m_keyboardStates, 0, sizeof(State) * NUM_SCANCODES);
	std::memset(m_mouseStates, 0, sizeof(State) * MOUSE_BUTTON_MAX);
	std::memset(m_mouseScrollStates, 0, sizeof(State) * SCROLL_MAX);
	std::memset(m_gameControllerButtonStates, 0, sizeof(State) * CONTROLLER_BUTTON_MAX);
	std::memset(m_gameControllerAxisStates, 0, sizeof(State) * CONTROLLER_AXIS_MAX);
	std::memset(m_gameControllerAxisStatesFloat, 0, sizeof(float) * CONTROLLER_AXIS_MAX);
}

InputManager::~InputManager()
{
	if (m_cursor) {
		SDL_DestroyCursor(m_cursor);
		m_cursor = nullptr;
	}
}

void InputManager::OpenGameController()
{
	int count = 0;
	SDL_JoystickID* gamepads = SDL_GetGamepads(&count);

	if (count > 0 && gamepads != nullptr)
	{

		m_gameController = SDL_OpenGamepad(gamepads[0]);
#ifdef _DEBUG
		if (m_gameController) {
			std::cerr << "Manette detectee : " << SDL_GetGamepadName(m_gameController) << "\n";
		}
#endif
		SDL_free(gamepads);
	}
#ifdef _DEBUG
	else {
		std::cerr << "Aucune manette branchee ou compatible\n";
	}
#endif
}
