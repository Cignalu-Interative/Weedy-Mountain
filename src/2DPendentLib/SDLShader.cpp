#include "pch.h"
#include "SDLShader.h"
#include "Window.h"

SDLShader* SDLShader::Get()
{
	static SDLShader instance;
	return &instance;
}

SDLShader::SDLShader()
{

}

SDLShader::~SDLShader()
{

}

SDL_GPUShaderFormat SDLShader::GetShaderFormat() const
{
	if (m_gpuDevice == nullptr)
	{
		SDL_Log("Lib/SDLShader.cpp/GetShaderFormat/m_gpuDevice is nullptr");
		return SDL_GPU_SHADERFORMAT_INVALID;
	}
	SDL_GPUShaderFormat shaderFormat = SDL_GetGPUShaderFormats(m_gpuDevice);
	return shaderFormat;
}

void SDLShader::CreateShader(const std::string& p_vs, const std::string& p_ps)
{
	{
		size_t sizeVs = 0;
		void* dataVs = SDL_LoadFile(p_vs.c_str(), &sizeVs);
		if (dataVs == NULL)
		{
			SDL_Log("Lib/SDLShader.cpp/LoadData/SDL_LoadFile Vs failed: %s", SDL_GetError());
			return;
		}

		m_shaderVsCreateInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
		m_shaderVsCreateInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
		m_shaderVsCreateInfo.code = (const Uint8*)dataVs;
		m_shaderVsCreateInfo.code_size = sizeVs;
		m_shaderVsCreateInfo.entrypoint = "main";

		m_ShadersVs = SDL_CreateGPUShader(m_gpuDevice, &m_shaderVsCreateInfo);
		if (m_ShadersVs == NULL)
		{
			SDL_Log("Lib/SDLShader.cpp/CreateShader/SDL_CreateGPUShader Vs failed: %s", SDL_GetError());
			return;
		}
		
		SDL_free(dataVs);
	}

	{
		size_t sizePs = 0;
		void* dataPs = SDL_LoadFile(p_ps.c_str(), &sizePs);
		if (dataPs == NULL)
		{
			SDL_Log("Lib/SDLShader.cpp/LoadData/SDL_LoadFile Ps failed: %s", SDL_GetError());
			return;
		}

		m_shaderPsCreateInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
		m_shaderPsCreateInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
		m_shaderPsCreateInfo.code = (const Uint8*)dataPs;
		m_shaderPsCreateInfo.code_size = sizePs;
		m_shaderPsCreateInfo.entrypoint = "main";

		m_ShadersPs = SDL_CreateGPUShader(m_gpuDevice, &m_shaderPsCreateInfo);
		if (m_ShadersPs == NULL)
		{
			SDL_Log("Lib/SDLShader.cpp/CreateShader/SDL_CreateGPUShader Ps failed: %s", SDL_GetError());
			return;
		}
		
		SDL_free(dataPs);
	}

	CreatePipeline();
	CreateCommandBuffer();
	GetWindowTexture();
}

void SDLShader::CreatePipeline()
{
	SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};

	pipelineInfo.vertex_shader = m_ShadersVs;
	pipelineInfo.fragment_shader = m_ShadersPs;

	SDL_GPUColorTargetDescription colorTarget = {};
	colorTarget.format = m_textureFormat;
	colorTarget.blend_state.enable_blend = false;

	pipelineInfo.target_info.color_target_descriptions = &colorTarget;
	pipelineInfo.target_info.num_color_targets = 1;

	pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

	pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	pipelineInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;

	pipelineInfo.vertex_input_state.num_vertex_buffers = 0;
	pipelineInfo.vertex_input_state.num_vertex_attributes = 0;

	m_pipeline = SDL_CreateGPUGraphicsPipeline(m_gpuDevice, &pipelineInfo);

	if (m_pipeline == NULL)
	{
		SDL_Log("Lib/SDLShader.cpp/CreatePipeline failed: %s", SDL_GetError());
		return;
	}
}

void SDLShader::CreateCommandBuffer()
{
	m_commandBuffer = SDL_AcquireGPUCommandBuffer(m_gpuDevice);
	if (m_commandBuffer == NULL)
	{
		SDL_Log("Lib/SDLShader.cpp/CreateCommandBuffer failed: %s", SDL_GetError());
		return;
	}
}

void SDLShader::GetWindowTexture()
{
	if (SDL_AcquireGPUSwapchainTexture(m_commandBuffer, m_window->GetWindow(), &m_swapchainTexture, nullptr, nullptr) == false)
	{
		SDL_Log("Impossible d'acquérir la texture de l'écran : %s", SDL_GetError());
		SDL_SubmitGPUCommandBuffer(m_commandBuffer);
		return;
	}
}

void SDLShader::Draw()
{
	SDL_GPUColorTargetInfo colorTargetInfo = {};
	colorTargetInfo.texture = m_swapchainTexture;
	colorTargetInfo.clear_color = { 0.1f, 0.1f, 0.1f, 1.0f }; // Gris foncé
	colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;          // Vide l'écran avant de dessiner
	colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;        // Enregistre ce qu'on va dessiner

	SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(m_commandBuffer, &colorTargetInfo, 1, nullptr);

	// 1. On applique notre PSO (Shaders + configurations)
	SDL_BindGPUGraphicsPipeline(renderPass, m_pipeline);

	// 2. On dessine ! (3 sommets, 1 instance, premier sommet à 0, première instance à 0)
	SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

	SDL_EndGPURenderPass(renderPass);
	//SDL_SubmitGPUCommandBuffer(m_commandBuffer); // Soumission à la carte graphique !
}
