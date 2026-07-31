#pragma once
//#include "glad/glad.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Vector2.h"
//Version 4.3.0

class Window
{
private:

	SDL_Window* m_window = nullptr; //Pointeur qui stock la window

	MIX_Mixer* m_mixer = nullptr; //Pointeur qui stock le mixer

	SDL_DisplayMode m_displayMode; //Struct qui stock différente informations de l'écran

	Vector2i m_windowDimension; //Dimention de la window

	bool m_isFullScreen; //Boll pour savoir si la window est en plein écran

	static constexpr float SCREEN_SCALE = 1.5f; // Taille de la window au lancement du jeu, elle est multiplié par la taille de l'écran pour obtenir la taille de la window

public:

	//Liste des class amie avec la class Window
	friend class Render;
	friend class Circle;
	friend class Rectangle;
	friend class Debug;
	friend class Sprite;
	friend class Entity;
	friend class Point;
	friend class Line;
	friend class Text;
	friend class Camera;

	//Destructeur de window
	~Window();

	//Creation d'un fenetre de jeu
	bool CreateWindow(); 
	//Sert à connaitre le format de pixels les dimention et fps de l'écran
	int V_Sync(); 
	//Change la valeur de m_isFullScreen si true alors false si false alors true
	void BoolFullScreen(); 
	//Définition de la taille de la fenetre de jeu
	void SetWindowSize(Vector2i p_dimention); 
	//Obtention de la taille de la fenetre de jeu
	Vector2i GetWindowSize(); 
	//Obtenir la moitié de la dimension de la window
	Vector2i GetHalfWindowSize();
	//Obtention de la fenetre de jeu en cours
	SDL_Window* GetWindow() const { return m_window; }
	//Obtention du DisplayMode
	SDL_DisplayMode GetDisplayMode() const { return m_displayMode; }
	//Obtention du mixer
	MIX_Mixer* GetMixer() const { return m_mixer; }

	void SetWindowAlwaysOnTop(bool p_alwaysOnTop);
	bool GetIsAlwaysOnTop() const { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_ALWAYS_ON_TOP; }

	void SetWindowOpacity(float p_opacity);
	float GetWindowOpacity() const { return SDL_GetWindowOpacity(m_window); }

	// 0 = arrete le clignotement, 1 = une fois, 2 = jusqu'a ce que l'utilisateur revienne sur la window
	void SetFlashWindow(int p_flash);


};