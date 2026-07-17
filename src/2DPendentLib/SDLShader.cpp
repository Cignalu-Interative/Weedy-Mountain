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

		m_shaderVsCreateInfo.num_samplers = 0;
		m_shaderVsCreateInfo.num_storage_textures = 0;
		m_shaderVsCreateInfo.num_storage_buffers = 0;
		m_shaderVsCreateInfo.num_uniform_buffers = 0;

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

		m_shaderPsCreateInfo.num_samplers = 1;
		m_shaderPsCreateInfo.num_storage_textures = 0;
		m_shaderPsCreateInfo.num_storage_buffers = 0;
		m_shaderPsCreateInfo.num_uniform_buffers = 0;

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

	pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

	pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	pipelineInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;

	SDL_GPUColorTargetDescription colorTarget = {};
	colorTarget.format = m_textureFormat;

	pipelineInfo.target_info.color_target_descriptions = &colorTarget;
	pipelineInfo.target_info.num_color_targets = 1;

	m_pipeline = SDL_CreateGPUGraphicsPipeline(
		m_gpuDevice,
		&pipelineInfo
	);

	if (!m_pipeline)
	{
		SDL_Log("Pipeline Error : %s", SDL_GetError());
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
		if (SDL_SubmitGPUCommandBuffer(m_commandBuffer) == false)
		{
			SDL_Log("Lib/SDLShader.cpp/GetWindowTexture/SDL_SubmitGPUCommandBuffer failed: %s", SDL_GetError());
			return;
		}
		return;
	}
}

void SDLShader::Draw(SDL_Texture* p_displayTexture)
{
	SDL_PropertiesID props = SDL_GetTextureProperties(p_displayTexture);
	SDL_GPUTexture* rawGpuTexture = (SDL_GPUTexture*)SDL_GetPointerProperty(props, SDL_PROP_TEXTURE_GPU_TEXTURE_POINTER, nullptr);

	// ==========================================
	// PHASE 3 : RENDU GPU 3D
	// ==========================================
	SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(m_gpuDevice);
	SDL_GPUTexture* swapchainTexture;

	if (SDL_AcquireGPUSwapchainTexture(cmdBuffer, m_window->GetWindow(), &swapchainTexture, nullptr, nullptr))
	{
		SDL_GPUColorTargetInfo colorTargetInfo = {};
		colorTargetInfo.texture = swapchainTexture;
		colorTargetInfo.clear_color = { 0.1f, 0.1f, 0.1f, 1.0f }; // Le fond gris de ton écran
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdBuffer, &colorTargetInfo, 1, nullptr);

		// 1. On active ton shader
		SDL_BindGPUGraphicsPipeline(renderPass, SDLShader::Get()->m_pipeline);

		// 2. On attache la texture extraite + le sampler
		SDL_GPUTextureSamplerBinding textureBinding = {};
		textureBinding.texture = rawGpuTexture; // La texture de géométrie !
		textureBinding.sampler = m_sampler;

		// (0 correspond au register(t0) / register(s0) du HLSL)
		SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);

		// 3. On dessine notre grand triangle qui couvrira l'écran

		if(m_pipeline == nullptr)
		{
			SDL_Log("Lib/SDLShader.cpp/Draw/m_pipeline is nullptr");
			return;
		}
		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(cmdBuffer);
}

void SDLShader::CreateSampler()
{
	m_samplerCreateInfo = {};
	m_samplerCreateInfo.min_filter = SDL_GPU_FILTER_LINEAR;
	m_samplerCreateInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
	m_samplerCreateInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
	m_samplerCreateInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	m_samplerCreateInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	m_samplerCreateInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	m_sampler = SDL_CreateGPUSampler(m_gpuDevice, &m_samplerCreateInfo);
	if (m_sampler == NULL)
	{
		SDL_Log("Lib/SDLShader.cpp/CreateSampler failed: %s", SDL_GetError());
		return;
	}
}
