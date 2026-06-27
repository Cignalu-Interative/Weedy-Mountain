#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

class Window;
class SDLShader
{
private:

	SDL_PropertiesID m_propertiesID = 0;

	SDL_GPUDevice* m_gpuDevice = nullptr; // Pointeur vers le GPU Device, utilisé pour gérer les shaders et les textures


public:

	static SDLShader* Get();

	SDLShader();
	~SDLShader();

	void InitializeGPUDevice(Window* p_window);

};

