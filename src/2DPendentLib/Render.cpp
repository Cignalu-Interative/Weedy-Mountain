#include "pch.h"
#include "Render.h"

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
	if (m_renderer != NULL)
	{
		SDL_DestroyRenderer(m_renderer);
	}
	if (m_gpuDevice != NULL)
	{
		SDL_DestroyGPUDevice(m_gpuDevice);
	}
}

void Render::CreatePropertiesGPU()
{
#ifdef NDEBUG
	if (SDL_SetBooleanProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, false) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetBooleanProperty/SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetBooleanProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN, false) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetBooleanProperty/SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN non reussie : %s", SDL_GetError());
		return;
	}
#endif
#ifdef _DEBUG
	if (SDL_SetBooleanProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, true) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetBooleanProperty/SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetBooleanProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN, true) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetBooleanProperty/SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN non reussie : %s", SDL_GetError());
		return;
	}
#endif
	if (SDL_SetBooleanProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_PREFERLOWPOWER_BOOLEAN, false) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetBooleanProperty/SDL_PROP_GPU_DEVICE_CREATE_PREFERLOWPOWER_BOOLEAN non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetStringProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING, "direct3d12") == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetStringProperty/SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetBooleanProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN, true) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetBooleanProperty/SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetNumberProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_VERSION_NUMBER, 619) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetNumberProperty/SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_VERSION_NUMBER non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetStringProperty(m_propertiesGPU_ID, SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_PATH_STRING, "../../lib/DLL_D3D12/") == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesGPU/SDL_SetStringProperty/SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_PATH_STRING non reussie : %s", SDL_GetError());
		return;
	}
}

void Render::CreatePropertiesRenderer(Window& f_window)
{
	if (SDL_SetStringProperty(m_propertiesRendererID, SDL_PROP_RENDERER_CREATE_NAME_STRING, "gpu") == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesRenderer/SDL_SetStringProperty/SDL_PROP_RENDERER_CREATE_NAME_STRING non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetPointerProperty(m_propertiesRendererID, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, f_window.GetWindow()) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesRenderer/SDL_SetPointerProperty/SDL_PROP_RENDERER_CREATE_WINDOW_POINTER non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetNumberProperty(m_propertiesRendererID, SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER, SDL_COLORSPACE_RGB_DEFAULT) == false) // ou SDL_COLORSPACE_SRGB_LINEAR
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesRenderer/SDL_SetNumberProperty/SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetNumberProperty(m_propertiesRendererID, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 0) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesRenderer/SDL_SetNumberProperty/SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetPointerProperty(m_propertiesRendererID, SDL_PROP_RENDERER_CREATE_GPU_DEVICE_POINTER, m_gpuDevice) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesRenderer/SDL_SetPointerProperty/SDL_PROP_RENDERER_CREATE_GPU_DEVICE_POINTER non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetBooleanProperty(m_propertiesRendererID, SDL_PROP_RENDERER_CREATE_GPU_SHADERS_DXIL_BOOLEAN, true) == false)
	{
		SDL_Log("Lib/Render.cpp/CreatePropertiesRenderer/SDL_SetBooleanProperty/SDL_PROP_RENDERER_CREATE_GPU_SHADERS_DXIL_BOOLEAN non reussie : %s", SDL_GetError());
		return;
	}
}

bool Render::CreateRenderer(Window& f_window)
{
	m_window = &f_window;

	CreatePropertiesGPU();
	m_gpuDevice = SDL_CreateGPUDeviceWithProperties(m_propertiesGPU_ID);
	if (m_gpuDevice == NULL)
	{
		SDL_Log("Lib/SDLShader.cpp/InitializeGPUDevice/SDL_CreateGPUDeviceWithProperties failed: %s", SDL_GetError());
		return false;
	}
	CreatePropertiesRenderer(f_window);
	//m_renderer = SDL_CreateRenderer(f_window.m_window, NULL);
	m_renderer = SDL_CreateRendererWithProperties(m_propertiesRendererID);
	if (m_renderer == NULL)
	{
		SDL_Log("Lib/Render.cpp/CreateRenderer/SDL_CreateRenderer/SDL_CreateRenderer non reussie : %s", SDL_GetError());
		return false;
	}

	m_displayTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, f_window.GetWindowSize().GetX(), f_window.GetWindowSize().GetY());

	std::cerr << "Renderer: " << SDL_GetRendererName(m_renderer) << std::endl;
	std::cerr << "texture format: " << SDL_GetGPUSwapchainTextureFormat(m_gpuDevice, f_window.GetWindow()) << std::endl;
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

void Render::SetRenderTarget()
{
	if (SDL_SetRenderTarget(m_renderer, m_displayTexture) == false)
	{
		SDL_Log("Lib/Render.cpp/SetRenderTarget/SDL_SetRenderTarget non reussie : %s", SDL_GetError());
		return;
	}
}

void Render::FlushRender()
{
	if (SDL_FlushRenderer(m_renderer) == false)
	{
		SDL_Log("Lib/Render.cpp/FlushRender/SDL_FlushRenderer non reussie : %s", SDL_GetError());
		return;
	}
}
