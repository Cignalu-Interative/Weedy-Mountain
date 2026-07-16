#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

class Window;
class SDLShader
{
private:

	SDL_GPUDevice* m_gpuDevice = nullptr; // Pointeur vers le GPU Device, utile pour gérer les shaders et les textures

public:

	static SDLShader* Get();

	void SetGPUDevice(SDL_GPUDevice* p_gpuDevice) { m_gpuDevice = p_gpuDevice; }

	SDLShader();
	~SDLShader();

	// Retourne le format de shader utilis par le GPU Device
	// Sert pour savoir quel type de shader utiliser (GLSL, HLSL, etc.) en fonction de la plateforme et du GPU
	// Retourne SDL_GPU_SHADERFORMAT_INVALID si le GPU Device n'est pas initialis
	SDL_GPUShaderFormat GetShaderFormat() const;
};

