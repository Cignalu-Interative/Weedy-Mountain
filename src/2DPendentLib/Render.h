#pragma once
#include <SDL.h>
//Version 2.2.0
class Window;
class Render
{
private:

	//Pointeur qui stock le render
	SDL_Renderer* m_renderer = nullptr;

public:

	~Render(); //Destructeur du render

	//Creation d'un render sur la m_window
	bool CreateRenderer(Window& f_window); 
	//Clear le render en cours
	void ClearRenderer(); 
	//Affiche le render en cours
	void PresentRenderer(); 
	//Obtention du render
	SDL_Renderer* GetRenderer() const { return m_renderer; }

	//Liste des class amie avec la class Render
	friend class Circle;
	friend class Rectangle;
	friend class Debug;
	friend class Sprite;
	friend class Entity;
	friend class Point;
	friend class Line;
	friend class Text;
	friend class AssetManager;
	friend class RectangleCollider;
	friend class Camera;
	friend class MapTiled;
};

