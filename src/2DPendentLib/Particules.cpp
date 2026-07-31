#include "pch.h"
#include "Particules.h"

#include "SDL3/SDL.h"

#include "Render.h"
#include "Camera.h"
#include "Window.h"

#include "GPUDevice.h"

Particles::Particles(const std::string& p_computeShaderName, Window* p_window, SDL_Texture* texture, Vector2f p_spawnerPos){
	m_pos = p_spawnerPos;
	Create(p_window->GetGPUDevice(), p_computeShaderName);
	m_particlesTexture = texture;
	Init();
}

void Particles::Create(GPUDevice* p_gpuDevice, const std::string& p_computeShaderName) {
	m_linkedGpuDevice = p_gpuDevice;
	if (m_linkedGpuDevice == nullptr) {
		std::cerr << "Failed to create particles, GpuDevice nullptr" << std::endl;
		return;
	}
	m_computePipeline = p_gpuDevice->GetComputePipeline(p_computeShaderName);
	if (m_computePipeline == nullptr) {
		std::cerr << "Failed to create particles pipeline" << std::endl;
		return;
	}
}

void Particles::Init() {
	m_bufferCapacity = 256;
	m_particleCount = static_cast<Uint32>(m_particles.size());

	Uint32 bufferByteSize = m_bufferCapacity * sizeof(Particle);

	SDL_GPUBufferCreateInfo bufferInfo{};
	bufferInfo.usage = SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE;
	bufferInfo.size = bufferByteSize;
	m_particlesBuffer = SDL_CreateGPUBuffer(m_linkedGpuDevice->m_gpuDevice, &bufferInfo);

	SDL_GPUTransferBufferCreateInfo uploadInfo{};
	uploadInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	uploadInfo.size = bufferByteSize;
	m_uploadTransfer = SDL_CreateGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, &uploadInfo);

	SDL_GPUTransferBufferCreateInfo downloadInfo{};
	downloadInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
	downloadInfo.size = bufferByteSize;
	m_downloadTransfer = SDL_CreateGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, &downloadInfo);

	if (!m_particlesBuffer || !m_uploadTransfer || !m_downloadTransfer) {
		std::cerr << "Failed to create GPU buffers: " << SDL_GetError() << std::endl;
		return;
	}

	if (m_particleCount > 0) {
		UploadParticles();
	}

	m_bufferInitialized = true;
}

void Particles::Draw(Render* p_render, Window* p_window, Camera* p_camera){
	if (m_particles.size() == 0) {
		return;
	}
	Vector2f camPos1 = p_camera->GetPosition({ 0.0f, 0.0f });
	Vector2f camPos2 = p_camera->GetPosition({ 1.0f, 1.0f });
	if (m_particlesTexture == nullptr) {
		std::cerr << "Particle texture is unavailable" << std::endl;
		return;
	}
	if (m_particlesTexture->h == 0 || m_particlesTexture->w == 0) {
		std::cerr << "Unvalid texture for particles" << std::endl;
		return;
	}
	Vector2f TextureSize = { static_cast<int>(m_particlesTexture->h) * m_scale.GetX() , static_cast<int>(m_particlesTexture->w) *  m_scale.GetY()};
	float margin = 20.0f;

	for (auto it = m_particles.begin(); it != m_particles.end();) {
		Particle& particle = *it;
		Vector2f pos = p_camera->WorldToScreen(particle.pos);
		bool inWindow = pos.GetX() - TextureSize.GetX()/2.0f - margin > camPos1.GetX() && pos.GetY() - TextureSize.GetY()/2.0f - margin > camPos1.GetY() || pos.GetX() + TextureSize.GetX()/2.0f + margin < camPos2.GetX() && pos.GetY() + TextureSize.GetY()/2.0f + margin < camPos2.GetY();
		if (inWindow) {
			SDL_FRect dstRect{ pos.GetX() - TextureSize.GetX() / 2.0f, pos.GetY() - TextureSize.GetY() / 2.0f, TextureSize.GetX() ,TextureSize.GetY()};
			SDL_RenderTexture(p_render->GetRenderer(), m_particlesTexture, nullptr, &dstRect);
		}

		if (particle.life <= 0) {
			it = m_particles.erase(it);
		}
		else {
			it++;
		}

	}
}

void Particles::Update(float deltaTime) {
	if (m_particles.size() == 0) {
		return;
	}
	if (!m_bufferInitialized || m_linkedGpuDevice == nullptr) return;

	if (m_uploadedCount < m_particleCount) {
		UploadRange(m_uploadedCount, m_particleCount - m_uploadedCount);
		m_uploadedCount = m_particleCount;
	}
	Uint32 bufferSize = m_particleCount * sizeof(Particle);

	SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(m_linkedGpuDevice->m_gpuDevice);

	SDL_GPUStorageBufferReadWriteBinding rwBinding{};
	rwBinding.buffer = m_particlesBuffer;
	rwBinding.cycle = false;

	SDL_GPUComputePass* computePass = SDL_BeginGPUComputePass(cmdBuffer, nullptr, 0, &rwBinding, 1);
	SDL_BindGPUComputePipeline(computePass, m_computePipeline);

	struct {
		float deltaTime; 
		Uint32 particleCount;
	} params = { deltaTime, m_particleCount };

	SDL_PushGPUComputeUniformData(cmdBuffer, 0, &params, sizeof(params));

	Uint32 threadsPerGroup = 64;
	Uint32 numGroups = (m_particleCount + threadsPerGroup - 1) / threadsPerGroup;
	SDL_DispatchGPUCompute(computePass, numGroups, 1, 1);

	SDL_EndGPUComputePass(computePass);

	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

	SDL_GPUBufferRegion source{};
	source.buffer = m_particlesBuffer;
	source.offset = 0;
	source.size = bufferSize;

	SDL_GPUTransferBufferLocation dest{};
	dest.transfer_buffer = m_downloadTransfer;
	dest.offset = 0;

	SDL_DownloadFromGPUBuffer(copyPass, &source, &dest);
	SDL_EndGPUCopyPass(copyPass);

	SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdBuffer);
	SDL_WaitForGPUFences(m_linkedGpuDevice->m_gpuDevice, true, &fence, 1);
	SDL_ReleaseGPUFence(m_linkedGpuDevice->m_gpuDevice, fence);

	void* mapped = SDL_MapGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_downloadTransfer, false);
	SDL_memcpy(m_particles.data(), mapped, bufferSize);
	SDL_UnmapGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_downloadTransfer);
}

void Particles::AddParticle(float p_offsetX, float p_offsetY, int p_id1, int p_id2, float p_life, float p_rotation){
	if (m_particleCount < m_particles.size()) {
		m_particles[m_particleCount] = Particle{ { m_pos.GetX() + p_offsetX, m_pos.GetY() + p_offsetY }, p_rotation, p_id1, p_id2, p_life };
	}
	else {
		m_particles.push_back(Particle{ { m_pos.GetX() + p_offsetX, m_pos.GetY() + p_offsetY }, p_rotation, p_id1, p_id2, p_life });
	}
	m_particleCount++;
	EnsureCapacity(m_particleCount);
}

void Particles::AddParticles(int nbr , Vector2f p_minOffset, Vector2f p_maxOffset, int p_minId, int p_maxId, float p_minLife, float p_maxLife){
	for (int i = 0; i < nbr; i++) {
		Vector2f pos{ p_minOffset.GetX() + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (p_maxOffset.GetX() - p_minOffset.GetX()))), p_minOffset.GetY() + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (p_maxOffset.GetY() - p_minOffset.GetY()))) };
		int id1 = p_minId + std::rand() % (p_maxId - p_minId);
		int id2 = p_minId + std::rand() % (p_maxId - p_minId);
		float life = p_minLife + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (p_maxLife - p_minLife)));
		AddParticle(pos.GetX(), pos.GetY(), id1, id2, life, 0.0f);
	}
}

void Particles::EnsureCapacity(Uint32 p_requiredCount) {
	if (p_requiredCount <= m_bufferCapacity) {
		return;
	}

	Uint32 newCapacity = m_bufferCapacity == 0 ? 64 : m_bufferCapacity;
	while (newCapacity < p_requiredCount) {
		newCapacity *= 2;
	}

	if (m_particlesBuffer) SDL_ReleaseGPUBuffer(m_linkedGpuDevice->m_gpuDevice, m_particlesBuffer);
	if (m_uploadTransfer) SDL_ReleaseGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_uploadTransfer);
	if (m_downloadTransfer) SDL_ReleaseGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_downloadTransfer);

	m_bufferCapacity = newCapacity;
	Uint32 bufferByteSize = m_bufferCapacity * sizeof(Particle);

	SDL_GPUBufferCreateInfo bufferInfo{};
	bufferInfo.usage = SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE;
	bufferInfo.size = bufferByteSize;
	m_particlesBuffer = SDL_CreateGPUBuffer(m_linkedGpuDevice->m_gpuDevice, &bufferInfo);

	SDL_GPUTransferBufferCreateInfo uploadInfo{};
	uploadInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	uploadInfo.size = bufferByteSize;
	m_uploadTransfer = SDL_CreateGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, &uploadInfo);

	SDL_GPUTransferBufferCreateInfo downloadInfo{};
	downloadInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
	downloadInfo.size = bufferByteSize;
	m_downloadTransfer = SDL_CreateGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, &downloadInfo);

	UploadParticles();
	m_uploadedCount = m_particleCount;	
}
	
void Particles::UploadParticles() {
	Uint32 bufferByteSize = m_particleCount * sizeof(Particle);

	void* mapped = SDL_MapGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_uploadTransfer, false);
	SDL_memcpy(mapped, m_particles.data(), bufferByteSize);
	SDL_UnmapGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_uploadTransfer);

	SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(m_linkedGpuDevice->m_gpuDevice);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

	SDL_GPUTransferBufferLocation source{};
	source.transfer_buffer = m_uploadTransfer;
	source.offset = 0;

	SDL_GPUBufferRegion dest{};
	dest.buffer = m_particlesBuffer;
	dest.offset = 0;
	dest.size = bufferByteSize;

	SDL_UploadToGPUBuffer(copyPass, &source, &dest, false);
	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(cmdBuffer);
}

void Particles::UploadRange(Uint32 p_startIndex, Uint32 p_count) {
	if (p_count == 0) return;

	Uint32 byteOffset = p_startIndex * sizeof(Particle);
	Uint32 byteSize = p_count * sizeof(Particle);

	void* mapped = SDL_MapGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_uploadTransfer, false);
	SDL_memcpy(mapped, m_particles.data() + p_startIndex, byteSize);
	SDL_UnmapGPUTransferBuffer(m_linkedGpuDevice->m_gpuDevice, m_uploadTransfer);

	SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(m_linkedGpuDevice->m_gpuDevice);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

	SDL_GPUTransferBufferLocation source{};
	source.transfer_buffer = m_uploadTransfer;
	source.offset = 0;

	SDL_GPUBufferRegion dest{};
	dest.buffer = m_particlesBuffer;
	dest.offset = byteOffset;
	dest.size = byteSize;

	SDL_UploadToGPUBuffer(copyPass, &source, &dest, false);
	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(cmdBuffer);
}

void Particles::CompactDeadParticle(){
	Uint32 i = 0;
	bool compacted = false;
	while (i < m_particleCount) {
		if (m_particles[i].life <= 0.0f) {
			m_particles[i] = m_particles[m_particleCount - 1];
			m_particleCount--;
			compacted = true;
		}
		else {
			i++;
		}
	}
	if (compacted) {
		m_uploadedCount = 0;
	}
}

void Particles::SetScale(Vector2f p_scale) {
	m_scale = p_scale;
}

bool Particles::IsEmpty() {
	return m_particles.size() == 0;
}

