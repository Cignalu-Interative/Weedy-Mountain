#pragma once
//#include "2DPendentLib/glm/glm.hpp"
#include <SDL3/SDL.h>
#include "Color.h"
//Version 2.2.0
class Window;
class Render
{
private:

	SDL_PropertiesID m_propertiesGPU_ID = 0;

	SDL_GPUDevice* m_gpuDevice = nullptr; // Pointeur vers le GPU Device, utilis pour grer les shaders et les textures


	SDL_PropertiesID m_propertiesRendererID = 0;

	//Pointeur qui stock le render
	SDL_Renderer* m_renderer = nullptr;

	Color m_backgroundColor;

public:

	Render();

	~Render(); //Destructeur du render

	void CreatePropertiesGPU(); //Creation des properties du GPU
	void CreatePropertiesRenderer(Window& f_window); //Creation des properties du render

	//Creation d'un render sur la m_window
	bool CreateRenderer(Window& f_window); 
	//Clear le render en cours
	void ClearRenderer(); 
	//Affiche le render en cours
	void PresentRenderer(); 

	SDL_GPUDevice* GetGPUDevice() const { return m_gpuDevice; }

	//Obtention du render
	SDL_Renderer* GetRenderer() const { return m_renderer; }

	//Clear le render en cours pour les éléments OpenGL,
	//cela permet d'éviter les problèmes de rendu lors du dessin d'éléments OpenGL et d'éléments SDL dans la même scène
	/*void ClearOpenGL();

	void RenderOpenGL(Window* f_window);*/

	void SetBackgroundColor(Color p_color);

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

