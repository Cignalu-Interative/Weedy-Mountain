#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <vector>
#include <unordered_map>

class Window;
class SDLShader
{
private:

	SDL_GPUDevice* m_gpuDevice = nullptr; // Pointeur vers le GPU Device, utilis pour grer les shaders et les textures

	SDL_GPUShader* m_ShadersVs = nullptr;
	SDL_GPUShader* m_ShadersPs = nullptr;

	SDL_GPUShaderCreateInfo m_shaderVsCreateInfo = {};
	SDL_GPUShaderCreateInfo m_shaderPsCreateInfo = {};

	SDL_GPUGraphicsPipeline* m_pipeline = nullptr;

	SDL_GPUTextureFormat m_textureFormat = {};

	SDL_GPUCommandBuffer* m_commandBuffer = nullptr;

	SDL_GPUTexture* m_swapchainTexture = nullptr;

	Window* m_window = nullptr; // Pointeur vers la window, utilis pour grer les shaders et les textures

	SDL_GPUSamplerCreateInfo m_samplerCreateInfo = {};

	SDL_GPUSampler* m_sampler = nullptr;

public:

	static SDLShader* Get();

	void SetGPUDevice(SDL_GPUDevice* p_gpuDevice) { m_gpuDevice = p_gpuDevice; }

	void SetTextureFormat(SDL_GPUTextureFormat p_textureFormat) { m_textureFormat = p_textureFormat; }

	void SetWindow(Window* p_window) { m_window = p_window; }

	SDLShader();
	~SDLShader();

	SDL_GPUShaderFormat GetShaderFormat() const;

	void CreateShader(const std::string& p_vs, const std::string& p_ps);

	void CreatePipeline();

	void CreateCommandBuffer();

	void GetWindowTexture();

	void Draw(SDL_Texture* p_displayTexture);

	void CreateSampler();
};

