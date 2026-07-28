#pragma once

#include <SDL3/SDL.h>

enum class ShaderType {
	Fragment,
	Vertex,
	Compute,
	Unknown
};

struct SDL_GPUDevice;


class Window;

class Shader{
private:
	SDL_GPUShader* m_shader = nullptr;
	ShaderType m_shaderType = ShaderType::Unknown;
	
public:

	~Shader();

	friend class GPUDevice;
};

