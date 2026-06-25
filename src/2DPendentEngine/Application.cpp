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
	InputManager::Get()->SetCursor("../../res/2DPendentEngine/Assets/Textures/Cursor/Basic/Default/pointer_c.png", { 0.0f, 0.0f });
	InputManager::Get()->SetWindow(&m_window);
	FontManager::Get()->AddFontByPath("../../res/2DPendentEngine/Assets/Fonts/DefaultPolice.ttf");
	SoundManager::Get()->SetMixer(m_window.GetMixer());
	Vector2i sizeWindow = m_window.GetWindowSize();
	m_camera = new Camera(sizeWindow);
	AssetManager::Get()->SetRenderer(m_render.GetRenderer());
	MapTiledManager::Get()->SetRender(&m_render);
	// La distance maximale à laquelle un son peut être entendu est définie comme la diagonale de la fenêtre, ce qui permet d'entendre les sons dans toute la fenêtre
	//SoundManager::Get()->SetMaxAudibleDistance(std::sqrtf(sizeWindow.GetX() * sizeWindow.GetX() + sizeWindow.GetY() * sizeWindow.GetY()));
	Debug::Get()->CreateText("AUTO_FPS", Vector2f(0.0f, 0.0f), "FPS : " + std::to_string(GetCurrentFPS()), Color(0, 255, 0, 255), true);
	m_timerFPS.SetCurrentTime();


	/*triangle.Init();
	shader.LoadFromFiles("../../src/2DPendentLib/Shaders/TestShader.vert", "../../src/2DPendentLib/Shaders/TestShader.frag");
	quadMesh.Upload(quadVerts, attribs, quadIndices);
	texture.LoadFromFile("../../res/2DPendentEngine/Assets/Textures/Other/Perceval.png");
	spriteOpenGL.Init(sizeWindow.GetX(), sizeWindow.GetY());*/

	/*for (int i = 0; i < 0; ++i)
	{
		TVVertex tvvertex;
		tvvertex.VertexSpecification();
		tvvertex.CreateGraphicsPipeline();
		m_tvVertexVector.push_back(tvvertex);
	}

	m_testTVVertex.VertexSpecification();
	m_testTVVertex.CreateGraphicsPipeline();
	m_testTVVertex.GetCameraOpenGL()->SetProjectionMatrix(glm::radians(45.0f), (float)sizeWindow.GetX() / (float)sizeWindow.GetY(), 0.1f, 10.0f);

	m_TVVertex2.VertexSpecification();
	m_TVVertex2.CreateGraphicsPipeline();
	m_TVVertex2.GetCameraOpenGL()->SetProjectionMatrix(glm::radians(45.0f), (float)sizeWindow.GetX() / (float)sizeWindow.GetY(), 0.1f, 10.0f);
	m_TVVertex2.SetTranslation(glm::vec3{ 0.0f,0.0f,-4.0f });

	std::cerr << "Count geometry for OpenGL: " << m_tvVertexVector.size() << std::endl;

	m_timeOpenGL.SetCurrentTime();
	
	InputManager::Get()->SetSDLWarpMouseInWindow(Vector2f(sizeWindow.GetX() / 2.0f, sizeWindow.GetY() / 2.0f));
	InputManager::Get()->SetRelativeMouseMode(true);*/

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
	/*triangle.Destroy();
	quadMesh.Destroy();*/


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

	//if (InputManager::Get()->IsKeyboardHeld(K_o))
	//{
	//	for (TVVertex tvvertx : m_tvVertexVector)
	//	{
	//		//tvvertx.SetUOffsetGL(m_testTVVertex.GetUOffesetGL() + 0.01f);
	//	}
	//	m_testTVVertex.SetTranslation(m_testTVVertex.GetTranslation() + glm::vec3{0.0f,0.0f,0.01f});
	//	m_TVVertex2.SetTranslation(m_TVVertex2.GetTranslation() + glm::vec3{ 0.0f,0.0f,0.01f });
	//	//m_testTVVertex.SetUOffsetGL(m_testTVVertex.GetUOffesetGL() + 0.01f);
	//	std::cerr << "g_uOffset K_o : " << m_testTVVertex.GetUOffesetGL() << std::endl;
	//}
	//if (InputManager::Get()->IsKeyboardHeld(K_l))
	//{
	//	for (TVVertex tvvertx : m_tvVertexVector)
	//	{
	//		//tvvertx.SetUOffsetGL(m_testTVVertex.GetUOffesetGL() - 0.01f);
	//	}
	//	m_testTVVertex.SetTranslation(m_testTVVertex.GetTranslation() - glm::vec3{ 0.0f,0.0f,0.01f });
	//	m_TVVertex2.SetTranslation(m_TVVertex2.GetTranslation() - glm::vec3{ 0.0f,0.0f,0.01f });
	//	//m_testTVVertex.SetUOffsetGL(m_testTVVertex.GetUOffesetGL() - 0.01f);
	//	std::cerr << "g_uOffset K_l : " << m_testTVVertex.GetUOffesetGL() << std::endl;
	//}
	//if (InputManager::Get()->IsKeyboardHeld(K_k))
	//{
	//	for (TVVertex tvvertx : m_tvVertexVector)
	//	{
	//		tvvertx.SetURotate(m_testTVVertex.GetURotate() + 0.80f);
	//	}
	//	m_testTVVertex.SetURotate(m_testTVVertex.GetURotate() + 0.80f);
	//	m_TVVertex2.SetURotate(m_TVVertex2.GetURotate() + 0.80f);
	//	std::cerr << "g_uRotate K_k : " << m_testTVVertex.GetURotate() << std::endl;
	//}
	//if (InputManager::Get()->IsKeyboardHeld(K_m))
	//{
	//	for (TVVertex tvvertx : m_tvVertexVector)
	//	{
	//		tvvertx.SetURotate(m_testTVVertex.GetURotate() - 0.80f);
	//	}
	//	m_testTVVertex.SetURotate(m_testTVVertex.GetURotate() - 0.80f);
	//	m_TVVertex2.SetURotate(m_TVVertex2.GetURotate() - 0.80f);
	//	std::cerr << "g_uRotate K_m : " << m_testTVVertex.GetURotate() << std::endl;
	//}

	//m_testTVVertex.GetCameraOpenGL()->MouseLook();

	//float speed = 0.025f;
	//if (InputManager::Get()->IsKeyboardHeld(K_z))
	//{
	//	m_testTVVertex.GetCameraOpenGL()->MoveForward(speed);
	//	m_TVVertex2.GetCameraOpenGL()->MoveForward(speed);
	//}
	//if (InputManager::Get()->IsKeyboardHeld(K_s))
	//{
	//	m_testTVVertex.GetCameraOpenGL()->MoveBackward(speed);
	//	m_TVVertex2.GetCameraOpenGL()->MoveBackward(speed);
	//}
	//if (InputManager::Get()->IsKeyboardHeld(K_q))
	//{
	//	m_testTVVertex.GetCameraOpenGL()->MoveLeft(speed);
	//	m_TVVertex2.GetCameraOpenGL()->MoveLeft(speed);
	//}
	//if (InputManager::Get()->IsKeyboardHeld(K_d))
	//{
	//	m_testTVVertex.GetCameraOpenGL()->MoveRight(speed);
	//	m_TVVertex2.GetCameraOpenGL()->MoveRight(speed);
	//}
	//std::cerr << "FPS : " + std::to_string(GetCurrentFPS()) << std::endl;

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
		//TO DO RE AUTOMATE 
		/*if (SoundManager::Get()->GetAutoMaxDistance())
		{
			SoundManager::Get()->SetMaxAudibleDistance(std::sqrtf(sizeWindow.GetX() * sizeWindow.GetX() + sizeWindow.GetY() * sizeWindow.GetY()));
		}*/
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
	m_render.ClearRenderer();
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	//Vector2i windowSize = GetWindow()->GetWindowSize();
	//glViewport(0, 0, windowSize.GetX(), windowSize.GetY());

	//m_render.ClearOpenGL();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_testTVVertex.GetTextureID());

	//for (TVVertex& tvvertex : m_tvVertexVector)
	//{
	//	glUseProgram(tvvertex.GetgGraphicsPipelineShaderProgram());

	//	float scale = m_testTVVertex.GetUScale();

	//	glm::mat4 model;
	//	model = glm::rotate(glm::mat4(1.0f), glm::radians(m_testTVVertex.GetURotate()), glm::vec3(0.0f, 1.0f, 0.0f));
	//	model = glm::translate(model, glm::vec3(0.0f, 0.0f, m_testTVVertex.GetUOffesetGL()));
	//	model = glm::scale(model, glm::vec3(scale, scale, scale));

	//	/*glm::mat4 model;
	//	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, m_testTVVertex.GetUOffesetGL()));
	//	model = glm::rotate(model, glm::radians(m_testTVVertex.GetURotate()), glm::vec3(0.0f, 1.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(scale, scale, scale));*/

	//	GLint u_ModelMatrixLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_ModelMatrix");

	//	if (u_ModelMatrixLocation >= 0)
	//	{
	//		glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
	//	}
	//	else
	//	{
	//		std::cerr << "Not find u_ModelMatrix" << std::endl;
	//	}

	//	/*glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)windowSize.GetX() / (float)windowSize.GetY(), 0.1f, 10.0f);

	//	GLint u_PerspectiveLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_Perspective");

	//	if (u_PerspectiveLocation >= 0)
	//	{
	//		glUniformMatrix4fv(u_PerspectiveLocation, 1, GL_FALSE, &perspective[0][0]);
	//	}
	//	else
	//	{
	//		std::cerr << "Not find u_Perspective" << std::endl;
	//	}*/

	//	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)windowSize.GetX() / (float)windowSize.GetY(), 0.1f, 10.0f);

	//	GLint u_ProjectionLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_Projection");

	//	if (u_ProjectionLocation >= 0)
	//	{
	//		glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
	//	}
	//	else
	//	{
	//		std::cerr << "Not find u_Perspective" << std::endl;
	//	}

	//	GLint u_ColorOffsetLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_ColorOffset");

	//	if (u_ColorOffsetLocation >= 0)
	//	{
	//		auto now = std::chrono::steady_clock::now();
	//		double elapsed = std::chrono::duration<double>(now - m_timeOpenGL.GetLastTime()).count();

	//		float offsetR = std::sin(elapsed) * 0.5f;          // Normal
	//		float offsetG = std::sin(elapsed + 1.0f) * 0.5f;   // Décalé de 1 radian
	//		float offsetB = std::sin(elapsed + 2.0f) * 0.5f;   // Décalé de 2 radians

	//		glUniform3f(u_ColorOffsetLocation, offsetR, offsetG, offsetB);
	//	}
	//	else {
	//		std::cerr << "Not find u_ColorOffset" << std::endl;
	//	}

	//	glBindVertexArray(tvvertex.GetgVertexArrayObject());
	//	//glBindBuffer(GL_ARRAY_BUFFER, m_tvVertex.GetgVertexBufferObject());

	//	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//}

	///////////////////////////////////////////////////////////////////////
	//glUseProgram(m_testTVVertex.GetgGraphicsPipelineShaderProgram());

	//CameraOpenGL* g_camera = m_testTVVertex.GetCameraOpenGL();


	///*glm::mat4 model;
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(m_testTVVertex.GetURotate()), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, m_testTVVertex.GetUOffesetGL()));
	//model = glm::scale(model, glm::vec3(scale, scale, scale));*/

	//
	//float scale = m_testTVVertex.GetUScale();

	//glm::mat4 model;
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(m_testTVVertex.GetTranslation().x, m_testTVVertex.GetTranslation().y, m_testTVVertex.GetTranslation().z));
	//model = glm::rotate(model, glm::radians(m_testTVVertex.GetURotate()), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(scale, scale, scale));

	//GLint u_ModelMatrixLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_ModelMatrix");

	//if (u_ModelMatrixLocation >= 0)
	//{
	//	glUniformMatrix4fv(u_ModelMatrixLocation, 1, false, &model[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_ModelMatrix" << std::endl;
	//}

	///*glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)windowSize.GetX() / (float)windowSize.GetY(), 0.1f, 10.0f);

	//GLint u_PerspectiveLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_Perspective");

	//if (u_PerspectiveLocation >= 0)
	//{
	//	glUniformMatrix4fv(u_PerspectiveLocation, 1, GL_FALSE, &perspective[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_Perspective" << std::endl;
	//}*/

	//glm::mat4 view = g_camera->GetViewMatrix();

	//GLint u_ViewLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_ViewMatrix");
	//if (u_ViewLocation >= 0)
	//{
	//	glUniformMatrix4fv(u_ViewLocation, 1, false, &view[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_ViewMatrix" << std::endl;
	//}

	//glm::mat4 perspective = g_camera->GetProjectionMatrix();
	//	
	///*glm::perspective(glm::radians(45.0f), (float)windowSize.GetX() / (float)windowSize.GetY(), 0.1f, 10.0f);*/

	//GLint u_ProjectionLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_Projection");
	//if (u_ProjectionLocation >= 0)
	//{
	//	glUniformMatrix4fv(u_ProjectionLocation, 1, false, &perspective[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_Perspective" << std::endl;
	//}

	//GLint u_ColorOffsetLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_ColorOffset");

	//if (u_ColorOffsetLocation >= 0)
	//{
	//	auto now = std::chrono::steady_clock::now();
	//	double elapsed = std::chrono::duration<double>(now - m_timeOpenGL.GetLastTime()).count();

	//	float offsetR = std::sin(elapsed) * 0.5f;          // Normal
	//	float offsetG = std::sin(elapsed + 1.0f) * 0.5f;   // Décalé de 1 radian
	//	float offsetB = std::sin(elapsed + 2.0f) * 0.5f;   // Décalé de 2 radians

	//	glUniform3f(u_ColorOffsetLocation, offsetR, offsetG, offsetB);
	//}
	//else {
	//	std::cerr << "Not find u_ColorOffset" << std::endl;
	//}

	/////////////////////////////////////////////////////////////////

	//glBindVertexArray(m_testTVVertex.GetgVertexArrayObject());
	////glBindBuffer(GL_ARRAY_BUFFER, m_testTVVertex.GetgVertexBufferObject());

	////glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);

	///////////////////////////////////////////////////////////////////////
	//glUseProgram(m_TVVertex2.GetgGraphicsPipelineShaderProgram());

	//CameraOpenGL* g_camera2 = m_TVVertex2.GetCameraOpenGL();

	//float scale2 = m_TVVertex2.GetUScale();

	///*glm::mat4 model;
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(m_testTVVertex.GetURotate()), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, m_testTVVertex.GetUOffesetGL()));
	//model = glm::scale(model, glm::vec3(scale, scale, scale));*/

	//glm::mat4 model2;
	//model2 = glm::translate(glm::mat4(1.0f), glm::vec3(m_TVVertex2.GetTranslation().x, m_TVVertex2.GetTranslation().y, m_TVVertex2.GetTranslation().z));
	//model2 = glm::rotate(model2, glm::radians(m_TVVertex2.GetURotate()), glm::vec3(0.0f, 1.0f, 0.0f));
	//model2 = glm::scale(model2, glm::vec3(scale2, scale2, scale2));

	//GLint u_ModelMatrixLocation2 = glGetUniformLocation(m_TVVertex2.GetgGraphicsPipelineShaderProgram(), "u_ModelMatrix");

	//if (u_ModelMatrixLocation2 >= 0)
	//{
	//	glUniformMatrix4fv(u_ModelMatrixLocation2, 1, false, &model2[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_ModelMatrix" << std::endl;
	//}

	///*glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)windowSize.GetX() / (float)windowSize.GetY(), 0.1f, 10.0f);

	//GLint u_PerspectiveLocation = glGetUniformLocation(m_testTVVertex.GetgGraphicsPipelineShaderProgram(), "u_Perspective");

	//if (u_PerspectiveLocation >= 0)
	//{
	//	glUniformMatrix4fv(u_PerspectiveLocation, 1, GL_FALSE, &perspective[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_Perspective" << std::endl;
	//}*/

	//glm::mat4 view2 = g_camera2->GetViewMatrix();

	//GLint u_ViewLocation2 = glGetUniformLocation(m_TVVertex2.GetgGraphicsPipelineShaderProgram(), "u_ViewMatrix");
	//if (u_ViewLocation2 >= 0)
	//{
	//	glUniformMatrix4fv(u_ViewLocation2, 1, false, &view2[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_ViewMatrix" << std::endl;
	//}

	//glm::mat4 perspective2 = glm::perspective(glm::radians(45.0f), (float)windowSize.GetX() / (float)windowSize.GetY(), 0.1f, 10.0f);

	//GLint u_ProjectionLocation2 = glGetUniformLocation(m_TVVertex2.GetgGraphicsPipelineShaderProgram(), "u_Projection");
	//if (u_ProjectionLocation2 >= 0)
	//{
	//	glUniformMatrix4fv(u_ProjectionLocation2, 1, false, &perspective2[0][0]);
	//}
	//else
	//{
	//	std::cerr << "Not find u_Perspective" << std::endl;
	//}

	//GLint u_ColorOffsetLocation2 = glGetUniformLocation(m_TVVertex2.GetgGraphicsPipelineShaderProgram(), "u_ColorOffset");

	//if (u_ColorOffsetLocation2 >= 0)
	//{
	//	auto now2 = std::chrono::steady_clock::now();
	//	double elapsed2 = std::chrono::duration<double>(now2 - m_timeOpenGL.GetLastTime()).count();

	//	float offsetR2 = std::sin(elapsed2) * 0.5f;          // Normal
	//	float offsetG2 = std::sin(elapsed2 + 1.0f) * 0.5f;   // Décalé de 1 radian
	//	float offsetB2 = std::sin(elapsed2 + 2.0f) * 0.5f;   // Décalé de 2 radians

	//	glUniform3f(u_ColorOffsetLocation2, offsetR2, offsetG2, offsetB2);
	//}
	//else {
	//	std::cerr << "Not find u_ColorOffset" << std::endl;
	//}

	/////////////////////////////////////////////////////////////////

	////glBindVertexArray(m_testTVVertex.GetgVertexArrayObject());
	//////glBindBuffer(GL_ARRAY_BUFFER, m_testTVVertex.GetgVertexBufferObject());

	//////glDrawArrays(GL_TRIANGLES, 0, 6);
	////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	////glBindVertexArray(0);
	////////////////////////////////////////////////////
	//glBindVertexArray(m_TVVertex2.GetgVertexArrayObject());
	////glBindBuffer(GL_ARRAY_BUFFER, m_testTVVertex.GetgVertexBufferObject());

	////glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);


	//glUseProgram(0);

	//glUseProgram(m_tvVertex.GetgGraphicsPipelineShaderProgram());

	//glBindVertexArray(m_tvVertex.GetgVertexArrayObject());
	////glBindBuffer(GL_ARRAY_BUFFER, m_tvVertex.GetgVertexBufferObject());

	////glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);
	//glUseProgram(0);

	////////////////////////////////////////////////////////////

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


	/*triangle.Draw();
	quadMesh.Draw();
	spriteOpenGL.DrawSprite(texture, 10, 10, 200, 200);*/

	m_render.PresentRenderer();
	//m_render.RenderOpenGL(&m_window);
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