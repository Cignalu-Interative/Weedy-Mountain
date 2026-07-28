#pragma once
#include <unordered_map>
#include <string>

#include "SDL3/SDL.h"

#include "Shader.h"

struct SDL_GPUDevice;
struct SDL_GPUComputePipeline;
class Window;


class GPUDevice {
	SDL_GPUDevice* m_gpuDevice;

	std::unordered_map<std::string, Shader*> m_shaders;
	std::unordered_map<std::string, SDL_GPUComputePipeline*> m_computeShaderPipelines;


private:
	Shader* LoadShader(const std::string& p_filename, int16_t p_sampleCount, int16_t p_bufferCount, int16_t p_storageBufferCount = 0, int16_t p_storageTextureCount = 0);
	SDL_GPUComputePipeline* LoadComputeShader(const std::string& p_filename, int16_t sampleCount = 0, int16_t bufferCount = 1, int16_t readStorageTexture = 0, int16_t readStorageBuffer = 0, int16_t readWriteStorageBuffer = 1, int16_t readWriteStorageTexture = 0);
	
	bool Init(Window* p_window);
	
	GPUDevice() = default;
	GPUDevice(Window* p_window);

	~GPUDevice();

	SDL_GPUComputePipeline* GetComputePipeline(const std::string& p_filename);

	friend class Window;

public:
	Shader* GetShader(const std::string& p_filename);
};