#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <functional>
#include "Keys.h"
#include "Vector2.h"
//Version 4.1.1
// faire des fonction pour détecter l'orientation des axis

class InputManager
{
public:

	//Singleton de l'InputManager
	static InputManager* Get();

	//Type de fonction pour le callback de quit
	using QuitCallback = std::function<void()>; 

		//Définir le callback de quit
	void SetQuitCallback(QuitCallback p_callback) 
	{
		m_quitCallback = p_callback;
	}

	//Met à jour les états des entrées en fonction des événements SDL
	void Update(); 
	
	//Detection si la touche du clavier est appuyer
	bool IsKeyboardDown(KeyboardKeycodeKeys p_key) const; 
	//Detection si la touche du clavier est maintenue
	bool IsKeyboardHeld(KeyboardKeycodeKeys p_key) const; 
	//Detection si la touche du clavier est relacher
	bool IsKeyboardUP(KeyboardKeycodeKeys p_key) const; 

	//Detection si la touche de la souris est appuyer
	bool IsMouseDown(Uint8 p_button) const; 
	//Detection si la touche de la souris est maintenue
	bool IsMouseHeld(Uint8 p_button) const; 
	//Detection si la touche de la souris est relacher
	bool IsMouseUp(Uint8 p_button) const; 

	//Detection si la touche de la souris est appuyer
	bool IsMouseScrollDown(MouseScroll p_scroll) const;
	//Detection si la touche de la souris est maintenue
	bool IsMouseScrollHeld(MouseScroll p_scroll) const;
	//Detection si la touche de la souris est relacher
	bool IsMouseScrollUp(MouseScroll p_scroll) const;
	//Detection si le bouton de la manette est appuyer
	bool IsGameControllerButtonDown(ControllerButton p_button) const; 
	//Detection si le bouton de la manette est maintenue
	bool IsGameControllerButtonHeld(ControllerButton p_button) const; 
	//Detection si le bouton de la manette est relacher
	bool IsGameControllerButtonUp(ControllerButton p_button) const; 

	//Detection si l'axe de la manette est en mouvement 
	bool IsGameControllerAxisDown(ControllerAxis p_axis) const; 
	//Detection si l'axe de la manette est en mouvement maintenue
	bool IsGameControllerAxisHeld(ControllerAxis p_axis) const; 
	//Detection si l'axe de la manette est en relacher
	bool IsGameControllerAxisUp(ControllerAxis p_axis) const; 

	//Detection de au moin un joystick en AxisDown
	bool IsJoyStickDown(JoyStick p_joystick) const; 
	//Detection de au moin un joystick en AxisHeld
	bool IsJoyStickHeld(JoyStick p_joystick) const; 
	//Detection de au moin un joystick en AxisUp
	bool IsJoyStickUp(JoyStick p_joystick) const; 

	//Obtention de la position X du curseur de la souris
	float GetMouseX(); 
	//Obtention de la position Y du curseur de la souris
	float GetMouseY(); 
	//Obtention de la position du curseur de la souris
	Vector2f GetMousePosition();

	int GetMouseWheelX();
	int GetMouseWheelY();
	Vector2i GetMouseWheel();

	//Detection d'une manette brancher
	void OpenGameController(); 

	//Obtention de l'orientation du joystick en fonction de la position des axes de la manette (retourne un vecteur normalisé avec une magnitude entre 0 et 1, ou un vecteur nul si le joystick est dans la zone morte)
	Vector2f GetJoyStickOrientation(JoyStick p_joystick); 

	void SetCursor(const std::string& path, Vector2f p_position);

#ifdef _DEBUG
	//Affichage des touches clavier / souris
	void DebugKeys(); 
	//Affichage des touches d'une manette
	void DebugControllerButtons(); 
	//Affichage des direction des joystick 
	void DebugControllerAxis(); 
#endif

private:

	//Struct pour stocker les états des entrées
	SDL_Event m_event = {};
	//Callback de quit
	QuitCallback m_quitCallback;
	//Struct pour stocker les états des entrées

	struct State
	{
		bool s_isDown;
		bool s_isHeld;
		bool s_isUp;
	};

	//Nombre de scancode de clavier avec leur état
	State m_keyboardStates[NUM_SCANCODES];

	//Nombre de bouton de souris
	static const int MOUSE_BUTTON_MAX = 6;
	// Nombres de bouton de souris avec leur état
	State m_mouseStates[MOUSE_BUTTON_MAX];
	//Nombre de scroll de souris avec leur état
	State m_mouseScrollStates[SCROLL_MAX];

	//Nombre de bouton de manette avec leur état
	State m_gameControllerButtonStates[CONTROLLER_BUTTON_MAX];
	//Nombre d'axe de manette avec leur état
	State m_gameControllerAxisStates[CONTROLLER_AXIS_MAX];
	//Stocker la valeur de chaque axe de manette pour pouvoir calculer l'orientation des joystick
	float m_gameControllerAxisStatesFloat[CONTROLLER_AXIS_MAX];

	Sint32 m_mouseWheelX = 0; // Stocker la valeur du scroll horizontal de la souris
	Sint32 m_mouseWheelY = 0; // Stocker la valeur du scroll vertical de la souris

	//Pointeur pour stocker la manette de jeu
	SDL_GameController* m_gameController = nullptr;

	SDL_Cursor* m_cursor = nullptr; //Pointeur pour stocker le curseur de la souris

	std::string m_cursorPath; //Chemin vers l'image du curseur de la souris

	static constexpr float AXIS_DEAD_ZONE = 0.15f; //Zone morte pour les axes de manette (valeurs entre -0.15 et 0.15 seront considérées comme 0)


	//Constructeur de InputManager
	InputManager()
	{
		std::memset(m_keyboardStates, 0, sizeof(State) * NUM_SCANCODES);
		std::memset(m_mouseStates, 0, sizeof(State) * MOUSE_BUTTON_MAX);
		std::memset(m_mouseScrollStates, 0, sizeof(State) * SCROLL_MAX);
		std::memset(m_gameControllerButtonStates, 0, sizeof(State) * CONTROLLER_BUTTON_MAX);
		std::memset(m_gameControllerAxisStates, 0, sizeof(State) * CONTROLLER_AXIS_MAX);
		std::memset(m_gameControllerAxisStatesFloat, 0, sizeof(float) * CONTROLLER_AXIS_MAX);
	}
	//Destructeur de InputManager
	~InputManager();
};