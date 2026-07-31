#pragma once
#include "Utils.h"

struct SDL_Texture;
struct SDL_GPUComputePipeline;
struct SDL_GPUBuffer;

class Window;
class Render;
class Camera;
class GPUDevice;

class SDL_GPUTransferBuffer;

struct Particle {
	Vector2f pos = { 0.0f, 0.0f };
	float rotation = 0.0f;
	int id1 = 0;
	int id2 = 0;
	float life = 1.0f;
};


class Particles{
	Vector2f m_pos;
	Vector2f m_scale = { 1.0f, 1.0f };
	
	SDL_Texture* m_particlesTexture = nullptr;
	
	std::vector<Particle> m_particles;
	SDL_GPUComputePipeline* m_computePipeline = nullptr;

	SDL_GPUBuffer* m_particlesBuffer = nullptr;
	Uint32 m_particleCount = 0;
	bool m_bufferInitialized = false;
	SDL_GPUTransferBuffer* m_uploadTransfer = nullptr;
	SDL_GPUTransferBuffer* m_downloadTransfer = nullptr;
	Uint32 m_uploadedCount = 0;
	Uint32 m_bufferCapacity = 0;

	GPUDevice* m_linkedGpuDevice = nullptr;

	void Create(GPUDevice* p_gpuDevice, const std::string& p_computeShaderName);
	void EnsureCapacity(Uint32 p_requiredCount);
	void UploadParticles();
	void CompactDeadParticle();
	void UploadRange(Uint32 p_startIndex, Uint32 p_count);

public:

	Particles(const std::string& p_computeShaderName, Window* p_window, SDL_Texture* texture, Vector2f p_spawnerPos);

	void Init();
	void Draw(Render* p_render, Window* p_window, Camera* p_camera);
	void Update(float deltaTime);

	void AddParticle(float p_offsetX, float p_offsetY, int p_id1, int p_id2, float p_life, float p_rotation = 0.0f);
	void AddParticles(int nbr, Vector2f p_minOffset, Vector2f p_maxOffset, int p_minId, int p_maxId, float p_minLife, float p_maxLife);
	void SetScale(Vector2f p_scale);

	bool IsEmpty();
};

