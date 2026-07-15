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
