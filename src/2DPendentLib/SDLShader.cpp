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
	m_propertiesID = SDL_CreateProperties();

#ifdef NDEBUG
		SDL_SetBooleanProperty(m_propertiesID, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, false);
#endif

}

SDLShader::~SDLShader()
{
	if (m_propertiesID)
	{
		SDL_DestroyProperties(m_propertiesID);
	}
}

void SDLShader::InitializeGPUDevice(Window* p_window)
{
	m_gpuDevice = SDL_CreateGPUDeviceWithProperties(m_propertiesID);

	if (m_gpuDevice == nullptr)
	{
		SDL_Log("Lib/SDLShader.cpp/InitializeGPUDevice/SDL_CreateGPUDeviceWithProperties failed: %s", SDL_GetError());
		return;
	}

	
	

}
