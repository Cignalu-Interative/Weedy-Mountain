#include "pch.h"
#include "GPUDevice.h"
#include "Window.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_gpu.h"

Shader* GPUDevice::LoadShader(const std::string& p_filename, int16_t p_sampleCount, int16_t p_bufferCount, int16_t p_storageBufferCount, int16_t p_storageTextureCount){
	if (m_shaders.contains(p_filename)) {
		return m_shaders[p_filename];
	}
	
	SDL_GPUShaderStage stage = {};
	Shader* shader = new Shader;
	if (SDL_strstr(p_filename.c_str(), ".vert")) {
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
		shader->m_shaderType = ShaderType::Vertex;
	}
	else if (SDL_strstr(p_filename.c_str(), ".frag")) {
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
		shader->m_shaderType = ShaderType::Fragment;
	}
	else if (SDL_strstr(p_filename.c_str(), ".comp")) {
		std::cerr << "can't load compute shader this way" << std::endl;
		delete shader;
		return nullptr;
	}
	else {
		std::cerr << "the shader has a wrong type" << std::endl;
		delete shader;
		return nullptr;
	}

	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(m_gpuDevice);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;

	const char* entrypoint;
	std::string path;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
#ifndef _DEBUG
		path = "../../../../res/shaders/Compiled/spirv/" + p_filename + ".spv";
#else
		path = "../../res/shaders/Compiled/spirv/" + p_filename + ".spv";
#endif // !_DEBUG
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
#ifndef _DEBUG
		path = "../../../../res/shaders/Compiled/dxil/" + p_filename + ".dxil";
#else
		path = "../../res/shaders/Compiled/dxil/" + p_filename + ".dxil";
#endif // !_DEBUG
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
#ifndef _DEBUG
		path = "../../../../res/shaders/Compiled/msl/" + p_filename + ".msl";
#else
		path = "../../res/shaders/Compiled/msl/" + p_filename + ".msl";
#endif // !_DEBUG		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main";
	}
	else {
		std::cerr << "unrockegnized shader compilation format " << std::endl;
		delete shader;
		return nullptr;
	}

	size_t codeSize;
	uint8_t* code = static_cast<uint8_t*>(SDL_LoadFile(path.c_str(), &codeSize));
	if (code == NULL) {
		std::cerr << "failed to load shader from disk " << path << std::endl;
	}

	uint8_t* castCode = code;
	SDL_GPUShaderCreateInfo shaderInfo = {};

	shaderInfo.code_size = codeSize;
	shaderInfo.code = castCode;
	shaderInfo.entrypoint = entrypoint;
	shaderInfo.format = format;
	shaderInfo.stage = stage;

	shaderInfo.num_samplers = p_sampleCount;
	shaderInfo.num_storage_textures = p_storageTextureCount;
	shaderInfo.num_storage_buffers = p_storageBufferCount;
	shaderInfo.num_uniform_buffers = p_bufferCount;
	shaderInfo.props = 0;

	shader->m_shader = SDL_CreateGPUShader(m_gpuDevice, &shaderInfo);
	if (shader->m_shader == nullptr) {
		std::cerr << "Failed to create the shader" << std::endl;
		delete shader;
		SDL_free(code);
		return nullptr;
	}
	SDL_free(code);
	m_shaders[p_filename] = shader;
	return shader;
}

SDL_GPUComputePipeline* GPUDevice::LoadComputeShader(const std::string& p_filename, int16_t p_sampleCount, int16_t p_bufferCount, int16_t p_readStorageTexture, int16_t p_readStorageBuffer, int16_t p_readWriteStorageBuffer, int16_t p_readWriteStorageTexture){
	if (m_computeShaderPipelines.contains(p_filename)) {
		return m_computeShaderPipelines[p_filename];
	}

	if (!SDL_strstr(p_filename.c_str(), ".comp")) {
		std::cerr << "can't create compute pipeline without compute shader" << std::endl;
		return nullptr;
	}



	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(m_gpuDevice);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;

	const char* entrypoint;
	std::string path;
	
	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
#ifndef _DEBUG
		path = "../../../../res/shaders/Compiled/spirv/" + p_filename + ".spv";
#else
		path = "../../res/shaders/Compiled/spirv/" + p_filename + ".spv";
#endif // !_DEBUG
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
#ifndef _DEBUG
		path = "../../../../res/shaders/Compiled/dxil/" + p_filename + ".dxil";
#else
		path = "../../res/shaders/Compiled/dxil/" + p_filename + ".dxil";
#endif // !_DEBUG
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
#ifndef _DEBUG
		path = "../../../../res/shaders/Compiled/msl/" + p_filename + ".msl";
#else
		path = "../../res/shaders/Compiled/msl/" + p_filename + ".msl";
#endif // !_DEBUG		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main";
	}
	else {
		std::cerr << "unrockegnized shader compilation format " << std::endl;
		return nullptr;
	}

	size_t shaderByteSize = -1;
	void* shaderByteCode = SDL_LoadFile(path.c_str(), &shaderByteSize);

	if (!shaderByteCode) {
		std::cerr << "Failed to load compute shader" << p_filename << std::endl;
		return nullptr;
	}

	SDL_GPUComputePipelineCreateInfo computePipelineInfo{};

	computePipelineInfo.code = static_cast<uint8_t*>(shaderByteCode);
	computePipelineInfo.code_size = shaderByteSize;
	computePipelineInfo.entrypoint = "main";
	computePipelineInfo.format = format;

	computePipelineInfo.num_samplers = p_sampleCount;
	computePipelineInfo.num_readonly_storage_buffers = p_readStorageBuffer;
	computePipelineInfo.num_readonly_storage_textures = p_readStorageTexture;
	computePipelineInfo.num_readwrite_storage_buffers = p_readWriteStorageBuffer;
	computePipelineInfo.num_readwrite_storage_textures = p_readWriteStorageTexture;
	computePipelineInfo.num_uniform_buffers = p_bufferCount;

	computePipelineInfo.threadcount_x = 64;
	computePipelineInfo.threadcount_y = 1;
	computePipelineInfo.threadcount_z = 1;

	SDL_GPUComputePipeline* pipeline = SDL_CreateGPUComputePipeline(m_gpuDevice, &computePipelineInfo);
	SDL_free(shaderByteCode);
	if (pipeline == nullptr) {
		std::cerr << "failed to create compute pipeline " << SDL_GetError() << std::endl;
		return nullptr;
	}
	m_computeShaderPipelines[p_filename] = pipeline;
	return pipeline;

}

bool GPUDevice::Init(Window* p_window){
	m_gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
	if (m_gpuDevice == nullptr) {
		std::cerr << "GPU device failed to create " << SDL_GetError() << std::endl;
		return false;
	}

	bool success = SDL_ClaimWindowForGPUDevice(m_gpuDevice, p_window->GetWindow());
	if (!success) {
		std::cerr << "GPU device failed to link with SDL window " << SDL_GetError() << std::endl;
		return false;
	}

	if (SDL_WindowSupportsGPUPresentMode(m_gpuDevice, p_window->GetWindow(), SDL_GPU_PRESENTMODE_IMMEDIATE)) {
		bool ok = SDL_SetGPUSwapchainParameters(m_gpuDevice, p_window->GetWindow(), SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_IMMEDIATE);
		if (!ok) {
			std::cerr << "Failed to set IMMEDIATE present mode " << SDL_GetError() << std::endl;
		}
	}
	else {
		std::cout << "PRESENTMODE_IMMEDIATE not supported" << std::endl;
	}

}

GPUDevice::GPUDevice(Window* p_window){
	Init(p_window);
}

GPUDevice::~GPUDevice(){
	for (auto& shader : m_shaders) {
		if (shader.second != nullptr) {
			delete shader.second;
		}
	}	
}

SDL_GPUComputePipeline* GPUDevice::GetComputePipeline(const std::string& p_filename){
	if (m_computeShaderPipelines.contains(p_filename)) {
		return m_computeShaderPipelines[p_filename];
	}
	else {
		LoadComputeShader(p_filename);
	}
	return (m_computeShaderPipelines.contains(p_filename)) ? m_computeShaderPipelines[p_filename] : nullptr;
}

Shader* GPUDevice::GetShader(const std::string& p_filename){
	return (m_shaders.contains(p_filename)) ? m_shaders[p_filename] : nullptr;
}
