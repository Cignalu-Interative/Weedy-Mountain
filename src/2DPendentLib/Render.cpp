#include "pch.h"
#include "Render.h"
#include "Window.h"

Render::Render()
{
	m_propertiesGPU_ID = SDL_CreateProperties();
	if (m_propertiesGPU_ID == 0)
	{
		SDL_Log("Lib/Render.cpp/Render/SDL_CreateProperties failed: %s", SDL_GetError());
		return;
	}

	m_propertiesRendererID = SDL_CreateProperties();
	if (m_propertiesRendererID == 0)
	{
		SDL_Log("Lib/Render.cpp/Render/SDL_CreateProperties failed: %s", SDL_GetError());
		return;
	}

#ifdef NDEBUG
	SDL_SetBooleanProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, false);
#endif


	
	SetBackgroundColor(ColorsNamed::Black);
}

Render::~Render()
{
	if (m_propertiesGPU_ID)
	{
		SDL_DestroyProperties(m_propertiesGPU_ID);
	}
	if (m_propertiesRendererID)
	{
		SDL_DestroyProperties(m_propertiesRendererID);
	}
	if (m_gpuDevice != NULL)
	{
		SDL_DestroyGPUDevice(m_gpuDevice);
	}
	if (m_renderer != NULL)
	{
		SDL_DestroyRenderer(m_renderer);
	}
}

bool Render::CreateRenderer(Window& f_window)
{
	/*m_gpuDevice = SDL_CreateGPUDeviceWithProperties(m_propertiesGPU_ID);

	if (m_gpuDevice == NULL)
	{
		SDL_Log("Lib/SDLShader.cpp/InitializeGPUDevice/SDL_CreateGPUDeviceWithProperties failed: %s", SDL_GetError());
		return false;
	}*/

	m_renderer = SDL_CreateRenderer(f_window.m_window, NULL);
	//m_renderer = SDL_CreateRendererWithProperties(m_propertiesRendererID);
	if (m_renderer == NULL)
	{
		SDL_Log("Lib/Render.cpp/CreateRenderer/SDL_CreateRenderer/SDL_CreateRenderer non reussie : %s", SDL_GetError());
		return false;
	}
	std::cerr << "Renderer: " << SDL_GetRendererName(m_renderer) << std::endl;
	return true;
}

void Render::ClearRenderer()
{
	if (SDL_SetRenderDrawColor(m_renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a) == false)
	{
		SDL_Log("Lib/Render.cpp/ClearRenderer/SDL_SetRenderDrawColor/SDL_SetRenderDrawColor non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_RenderClear(m_renderer) == false)
	{
		SDL_Log("Lib/Render.cpp/ClearRenderer/SDL_RenderClear/SDL_RenderClear non reussie : %s", SDL_GetError());
		return;
	}
}

void Render::PresentRenderer()
{
	if (SDL_RenderPresent(m_renderer) == false)
	{
		SDL_Log("Lib/Render.cpp/PresentRenderer/SDL_RenderPresent non reussie : %s", SDL_GetError());
		return;
	}
}

//void Render::ClearOpenGL()
//{
//	glClearColor(m_OpenGLColor.r, m_OpenGLColor.g, m_OpenGLColor.b, m_OpenGLColor.a);
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//}

//void Render::RenderOpenGL(Window* f_window)
//{
//	SDL_GL_SwapWindow(f_window->GetWindow());
//}

void Render::SetBackgroundColor(Color p_backgroundColor)
{
	m_backgroundColor = p_backgroundColor;

	/*m_OpenGLColor = glm::vec4(
		m_backgroundColor.r / 255.f,
		m_backgroundColor.g / 255.f,
		m_backgroundColor.b / 255.f,
		m_backgroundColor.a / 255.f
	);*/
}
