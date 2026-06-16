#include "pch.h"
#include "Application.h"
#include "2DPendentLib/InputManager.h"
#include "Entity.h"
#include "Timer.h"
#include "Scene.h"
#include "2DPendentLib/Utils.h"
#include "MapTiledManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentLib/FontManager.h"
#include "CollisionManager.h"
#include "DialogueManager.h"
#include "PhysicsManager.h"

Application::Application()
{
	m_entities.reserve(256); // Réserver de la mémoire pour 256 entités afin d'éviter les reallocations fréquentes lors de l'ajout d'entités
	m_entityToDestroy.reserve(32); // Réserver de la mémoire pour 32 entités à détruire afin d'éviter les reallocations fréquentes lors de la destruction d'entités
	m_entityToAdd.reserve(32); // Réserver de la mémoire pour 32 entités à ajouter afin d'éviter les reallocations fréquentes lors de l'ajout d'entités
}

Application::~Application()
{
	Shutdown();
}

Application* Application::Get()
{
	static Application instance;
	return &instance;
}

void Application::Init()
{
	if (m_window.CreateWindow() == false)
	{
		return;
	}
	if (m_render.CreateRenderer(m_window) == false)
	{
		return;
	}

	if (m_window.GetWindow() != NULL && m_render.GetRenderer() != NULL)
	{
		m_isRunning = true;
	}
	Debug::Get()->SetRenderer(m_render.GetRenderer());
	Debug::SetDeltaTimeCallback([this]() {
		return m_deltaTime;
		});
	InputManager::Get()->SetQuitCallback([this]()
		{ m_isRunning = false; });
	FontManager::Get()->AddFontByPath("../../res/2DPendentEngine/Assets/Fonts/DefaultPolice.ttf");
	InputManager::Get()->SetCursor("../../res/2DPendentEngine/Assets/Textures/Cursor/Basic/Default/pointer_c.png", { 0.0f, 0.0f });
	Vector2i sizeWindow = m_window.GetWindowSize();
	m_camera = new Camera(sizeWindow);
	AssetManager::Get()->SetRenderer(m_render.GetRenderer());
	MapTiledManager::Get()->SetRender(&m_render);
	// La distance maximale à laquelle un son peut être entendu est définie comme la diagonale de la fenêtre, ce qui permet d'entendre les sons dans toute la fenêtre
	SoundManager::Get()->SetMaxAudibleDistance(std::sqrtf(sizeWindow.GetX() * sizeWindow.GetX() + sizeWindow.GetY() * sizeWindow.GetY()));
	Debug::Get()->CreateText("AUTO_FPS", Vector2f(0.0f, 0.0f), "FPS : " + std::to_string(GetCurrentFPS()), Color(0, 255, 0, 255), true);
	m_timerFPS.SetCurrentTime();

	std::cerr << "Application initialized successfully." << std::endl;
}

void Application::Run()
{
	while (m_isRunning)
	{
		FPSDeltaTime();

		CapFPS();

		UpdateFPSCounter();

		HandleInput();

		Update();

		Draw();

		//SceneManager::Get()->ChangeCurrentScene();
	}
}

void Application::Shutdown()
{
	//detruire toute les instances de chaque classe et les ressources
	for (Entity* entity : m_entities)
	{
		delete entity;
	}
	m_entities.clear();
	for (Entity* entity : m_entityToDestroy)
	{
		if (entity != nullptr)
		{
			delete entity;
		}
	}
	m_entityToDestroy.clear();
	for (Entity* entity : m_entityToAdd)
	{
		delete entity;
	}
	m_entityToAdd.clear();
	std::cerr << "Application is shutting down..." << std::endl;
}

void Application::FPSDeltaTime()
{
	if (m_drawFPS)
	{
		Debug::Get()->CreateText("AUTO_FPS", Vector2f(0.0f, 0.0f),
			"FPS : " + std::to_string(GetCurrentFPS()),
			Color(0, 255, 0, 255), true);
	}
	else
	{
		Debug::Get()->destroyText("AUTO_FPS");
	}
	if (m_autoFPS)
	{
		SetFPS(m_window.V_Sync());
		//SetFPS(240);
	}
}

void Application::HandleInput()
{
	InputManager::Get()->Update();
	
	if (SceneManager::Get()->IsSceneInStack() == false)
	{
		return;
	}

	SceneManager::Get()->EventScenesInStack();

	//SceneManager::Get()->GetCurrentScene()->OnEvent();


	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{
		Entity* entity = *it;

		entity->OnEvent();

		if (entity->ToDestroy() == false)
		{
			++it;
			continue;
		}

		m_entityToDestroy.push_back(entity);
		it = m_entities.erase(it);
	}
}

void Application::Update()
{
	/*if (SceneManager::Get()->GetCurrentScene() == nullptr)
	{
		return;
	}*/


	if (SceneManager::Get()->IsSceneInStack() == false)
	{
		return;
	}

	SceneManager::Get()->UpdateScenesInStack();

	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{
		Entity* entity = *it;

		entity->Update();

		if (entity->ToDestroy() == false)
		{
			++it;
			continue;
		}
		entity->GetScene()->RemoveEntity(entity);
		m_entityToDestroy.push_back(entity);
		it = m_entities.erase(it);
	}

	IsEneableToUpdate();

	SoundManager::Get()->Update();

	Vector2i sizeWindow = m_window.GetWindowSize();
	if (m_camera->GetDimension().GetX() != sizeWindow.GetX() || m_camera->GetDimension().GetY() != sizeWindow.GetY())
	{
		m_camera->SetDimension(m_window.GetWindowSize());
		if (SoundManager::Get()->GetAutoMaxDistance())
		{
			SoundManager::Get()->SetMaxAudibleDistance(std::sqrtf(sizeWindow.GetX() * sizeWindow.GetX() + sizeWindow.GetY() * sizeWindow.GetY()));
		}
	}

	for (auto it = m_entityToDestroy.begin(); it != m_entityToDestroy.end(); ++it)
	{
		delete* it;
		*it = nullptr;
	}
	m_entityToDestroy.clear();

	for (auto it = m_entityToAdd.begin(); it != m_entityToAdd.end(); ++it)
	{
		m_entities.push_back(*it);
	}
	m_entityToAdd.clear();

	if (m_cameraIsClamp)
		ClampCameraToCurrentMap();
}

void Application::FixedUpdate()
{
	// Ordre impératif de la fixed-loop physique :
	// 1. Intégrer les forces (gravité, damping) -> met à jour les vélocités
	PhysicsManager::Get()->IntegrateForces(m_FIX_UPDATE);

	// 2. Synchroniser les colliders sur les positions actuelles des entités
	CollisionManager::Get()->SyncAll();

	// 3. Détecter les collisions, résoudre (répulsion + impulsions), fire les callbacks
	//    CollisionManager appelle PhysicsManager::Repulse et ResolveCollision en interne
	CollisionManager::Get()->Update();

	// 4. Appliquer les vélocités corrigées -> déplacer les entités
	PhysicsManager::Get()->IntegrateVelocities(m_FIX_UPDATE);
}

void Application::ClampCameraToCurrentMap()
{
	MapTiledManager* map = MapTiledManager::Get();

	if (map->GetCurrentMap() == nullptr)
		std::cerr << "CurrentMap is nullptr" << std::endl;

	Vector2f mapDimensions = (Vector2f)map->GetCurrentMap()->GetDimension();
	Vector2f cameraDimensions = (Vector2f)m_camera->GetDimension();

	Vector2f cameraPos = m_camera->GetPosition();

	Vector2f realCamDim = cameraDimensions / m_camera->GetZoom();

	Vector2f min = realCamDim / 2.f;
	Vector2f max = mapDimensions - min;

	Vector2f clampPosition = {}; // Utils::Clamp(cameraPos, Vector2f{}, max);

	clampPosition.SetX(std::clamp(cameraPos.GetX(), min.GetX(), max.GetX()));
	clampPosition.SetY(std::clamp(cameraPos.GetY(), min.GetY(), max.GetY()));

	m_camera->SetPosition(clampPosition);
}

void Application::Draw()
{
	m_render.ClearRenderer();

	MapTiledManager::Get()->DrawCurrentMap(&m_render, &m_window, m_camera);

	std::vector<Entity*> entToDraw = m_entities;

	std::sort(entToDraw.begin(), entToDraw.end(), [](const Entity* a, const Entity* b)
		{
			return a->GetDrawLayer() < b->GetDrawLayer();
		});

	for (Entity* entity : entToDraw)
	{
		entity->Draw(&m_render, &m_window);
	}

	Debug::Get()->Draw(&m_render, &m_window, m_camera);

	if (m_camera->GetIsShow())
	{
		m_camera->Draw(&m_render, &m_window, m_camera);
	}

	m_render.PresentRenderer();
}

void Application::SetFPS(int p_targetFPS)
{
	if (p_targetFPS <= 0)
	{
		return;
	}
	m_targetFPS = p_targetFPS;
	m_targetDeltaTime = 1.0f / m_targetFPS;
}

void Application::CapFPS()
{
	m_deltaTime = (float)m_timerFPS.GetElapsed();

	float sleepTime = m_targetDeltaTime - m_deltaTime; // 1M ns (temps de sommeil pour atteindre le temps cible entre chaque frame)

	if (sleepTime > 0.0f) // Si le temps de sommeil est sup�rieur � 1 ms, on dort pendant ce temps pour limiter le nombre de frame par seconde
	{
		m_timerFPS.Sleep(sleepTime);  // Dort pendant tant de nanosecondes pour limiter le nombre de frame par seconde
		m_deltaTime = m_targetDeltaTime;
	}
}

void Application::UpdateFPSCounter()
{
	if (m_deltaTime <= 0.0f)
	{
		return;
	}

	m_timeLastUpdateFPS -= m_deltaTime;
	m_frameCount++;

	if (m_timeLastUpdateFPS > 0.0f)
	{
		return;
	}

	m_currentFPS = m_frameCount;
	m_frameCount = 0;
	m_timeLastUpdateFPS = m_timeToUpdateFPS;
}

void Application::IsEneableToUpdate()
{
	m_accumulator += Utils::LimitMax(m_deltaTime, 0.05f);

	while (m_accumulator >= m_FIX_UPDATE)
	{
		FixedUpdate();
		m_accumulator -= m_FIX_UPDATE;
	}

}