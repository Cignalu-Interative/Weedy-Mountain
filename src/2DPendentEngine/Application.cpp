#include "pch.h"
#include "Application.h"
#include "2DPendentLib/InputManager.h"
#include "Entity.h"
#include "Timer.h"
#include "Scene.h"
#include "MapTiledManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentLib/FontManager.h"
#include "CollisionManager.h"



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



	triangle.Init();
	shader.LoadFromFiles("../../src/2DPendentLib/Shaders/TestShader.vert", "../../src/2DPendentLib/Shaders/TestShader.frag");
	quadMesh.Upload(quadVerts, attribs, quadIndices);
	

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

	triangle.Destroy();
	quadMesh.Destroy();
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

	CollisionManager::Get()->SyncAll();
	CollisionManager::Get()->Update();

	//SceneManager::Get()->UpdateCurrentScene();
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


	/*triangle.modelMatrix = glm::mat4(1.0f);

	triangle.modelMatrix = glm::translate(triangle.modelMatrix, glm::vec3(0.5f, 0.2f, 0.0f));*/
}

void Application::UpdatePhysics_Collisions()
{
	for (Entity* entity : m_entities)
	{
		//entity->UpdateMovement_Forces(m_FIX_UPDATE);
	}

	std::set<std::pair<Entity*, Entity*>> resolvedPairs;

	for (Entity* entity : m_entities)
	{
		entity->UpdateMovement_Velocity(m_FIX_UPDATE);
	}

	return;
	
	for (auto it1 = m_entities.begin(); it1 != m_entities.end(); ++it1)
	{
		auto it2 = it1;
		++it2;
		for (; it2 != m_entities.end(); ++it2)
		{
			Entity* entity = *it1;
			Entity* otherEntity = *it2;

			for (Collider* colliderA : entity->GetColliders())
			{
				for (Collider* colliderB : otherEntity->GetColliders())
				{
					if (!colliderA || !colliderB)
						continue;

					if (colliderA->IsTrigger() || colliderB->IsTrigger())
						continue;

					if (colliderA->CanCollideWith(colliderB) == false)  
						continue;

					if (Collision::IsColliding(*colliderA, *colliderB) == false)
						continue;

					if (entity->IsRigidBody() && otherEntity->IsRigidBody())
					{
						float dx = colliderB->GetPosition().GetX() - colliderA->GetPosition().GetX();
						float dy = colliderB->GetPosition().GetY() - colliderA->GetPosition().GetY();

						float overlapX = (colliderA->GetDimension().GetX() * 0.5f + colliderB->GetDimension().GetX() * 0.5f) - Utils::AbsValue(dx);
						float overlapY = (colliderA->GetDimension().GetY() * 0.5f + colliderB->GetDimension().GetY() * 0.5f) - Utils::AbsValue(dy);

						Vector2f normal;
						if (overlapX < overlapY)
							normal = { dx < 0 ? -1.0f : 1.0f, 0.0f };
						else
							normal = { 0.0f, dy < 0 ? -1.0f : 1.0f };

						if (normal.GetY() > 0.5f) entity->SetIsGrounded(true);
						if (normal.GetY() < -0.5f) otherEntity->SetIsGrounded(true);

						auto entityPair = std::make_pair(std::min(entity, otherEntity), std::max(entity, otherEntity));

						if (resolvedPairs.count(entityPair) == 0)
						{
							Collision::Repulse(entity, otherEntity);

							PhysicsBody* physicsBodyA = entity->m_physicsBody;
							PhysicsBody* physicsBodyB = otherEntity->m_physicsBody;

							if (physicsBodyA && physicsBodyB)
								PhysicsBody::ResolveCollisionPhysics(physicsBodyA, physicsBodyB, entity->GetPosition(), normal);

							resolvedPairs.insert(entityPair);
						}
					}
				}
			}
		}
	}

	for (Entity* entity : m_entities)
		entity->UpdateMovement_Velocity(m_deltaTime);
}

void Application::Draw()
{
	//m_render.ClearRenderer();
	m_render.ClearOpenGL();

	MapTiledManager::Get()->DrawCurrentMap(&m_render, &m_window, m_camera);

	for (Entity* entity : m_entities)
	{
		entity->Draw(&m_render, &m_window);
	}

	Debug::Get()->Draw(&m_render, &m_window, m_camera);

	if (m_camera->GetIsShow())
	{
		m_camera->Draw(&m_render, &m_window, m_camera);
	}



	triangle.Draw();
	quadMesh.Draw();


	//m_render.PresentRenderer();
	m_render.RenderOpenGL(&m_window);
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
	//0.05 secondes correspond à 20 FPS, cela permet d'éviter les problèmes de physique et de logique du jeu qui peuvent survenir
	// lorsque le temps entre deux frames est trop long (par exemple, lorsque le jeu est mis en pause ou lorsqu'il y a un ralentissement temporaire)
	// Vaut mieux un temps de mise à jour plus long que de sauter des mises à jour et de causer des problèmes de logique du jeu
	//double frametime = Utils::LimitMax((double)m_deltaTime, 0.05);
	m_accumulator += m_deltaTime;
	while (m_accumulator >= m_FIX_UPDATE)
	{
		UpdatePhysics_Collisions();
		m_accumulator -= m_FIX_UPDATE;
	}
}