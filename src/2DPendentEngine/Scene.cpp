#include "pch.h"
#include "Scene.h"
#include "Application.h"
#include "Entity.h"
#include "2DPendentLib/Shape.h"
#include "SoundManager.h"
#include "AssetManager.h"
#include "Camera.h"
#include "Debug.h"
#include "Geometry.h"
#include "CollisionManager.h"
#include "PhysicsManager.h"
#include "Button.h"
#include "Slider.h"
#include "TextBox.h"
#include "Particules.h"
#include "GPUDevice.h";

void Scene::Initialize()
{
	if (m_isInCache)
	{
		return;
	}
	OnInitialize();
	m_isInCache = true;
}

void Scene::Event()
{
	if (m_isSleepEvent)
	{
		return;
	}
	OnEvent();
}

void Scene::Update()
{
	if (m_isSleepUpdate)
	{
		return;
	}
	for (auto& p : m_particles) {	
		p->Update(GetDeltaTime());
	}
	OnUpdate();
}

void Scene::Sleep()
{
	if (m_isOnStateSleep)
	{
		return;
	}
	m_isOnStateSleep = true;
	m_isOnStateWake = false;

	for (Entity* entity : m_entities)
	{
		std::vector<Collider*> collider = entity->GetColliders(); 

		for(Collider* col : collider)
		{
			if (col != nullptr)
				CollisionManager::Get()->UnregisterCollider(col);
		}

		if (entity->GetPhysicsBody() != nullptr)
			PhysicsManager::Get()->UnregisterBody(entity);
	}

	OnSleep();
}

void Scene::Wake()
{
	if (m_isOnStateWake)
	{
		return;
	}
	m_isOnStateWake = true;
	m_isOnStateSleep = false;

	for (Entity* entity : m_entities)
	{
		std::vector<Collider*> collider = entity->GetColliders();

		for (Collider* col : collider)
		{
			if (col != nullptr)
				CollisionManager::Get()->RegisterCollider(col);
		}

		if (entity->GetPhysicsBody() != nullptr)
			PhysicsManager::Get()->RegisterBody(entity);
	}

	OnWake();
}

void Scene::Destroy()
{
	m_isInCache = false;

	for (Entity* entity : m_entities)
	{
		std::vector<Collider*> collider = entity->GetColliders();

		for (Collider* col : collider)
		{
			if (col != nullptr)
				CollisionManager::Get()->UnregisterCollider(col);
		}

		if (entity->GetPhysicsBody() != nullptr)
			PhysicsManager::Get()->UnregisterBody(entity);
	}

	OnDestroy();
}

void Scene::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	if (m_isSleepDraw)
	{
		return;
	}

	/*std::sort(entToDraw.begin(), entToDraw.end(), [](const Entity* a, const Entity* b)
	{
		return a->GetDrawLayer() < b->GetDrawLayer();
	});*/

	/*std::list<Entity*> entToDraw = m_entities;

	entToDraw.sort([](const Entity* a, const Entity* b)
	{
		return a->GetDrawLayer() < b->GetDrawLayer();
	});

	for (Entity* entity : entToDraw)
	{
		entity->Draw(f_render, f_window);
	}*/
	for (auto& p : m_particles) {
		p->Draw(f_render, f_window, p_camera);
	}

	OnDraw();
}

Button* Scene::CreateButton(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_textID, const std::string& p_text, bool p_isGUI)
{
	if (m_application == nullptr)
	{
		return nullptr;
	}

	Button* button = new Button();

	button->SetScene(this);
	button->Initialize(p_position, p_dimention, p_spritePath, m_application->GetRender(), Shape::Rectangle, p_isGUI, false, "");
	button->InitText(p_textID, p_text);
	m_application->m_entityToAdd.push_back(button);
	AddEntity(button);

	return button;
}

Slider* Scene::CreateSlider(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_id, int p_stateCount, bool p_isGUI)
{
	if (m_application == nullptr)
	{
		return nullptr;
	}

	Slider* slider = new Slider();

	slider->SetScene(this);
	slider->Initialize(p_position, p_dimention, p_spritePath, m_application->GetRender(), Shape::Rectangle, p_isGUI, false, "");
	slider->InitSlider(p_id, p_dimention.GetX(), p_dimention.GetY(), p_stateCount);
	m_application->m_entityToAdd.push_back(slider);
	AddEntity(slider);

	return slider;
}

TextBox* Scene::CreateTextBox(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI)
{
	if (m_application == nullptr)
	{
		return nullptr;
	}

	TextBox* textBox = new TextBox();

	textBox->SetScene(this);
	textBox->SetWindow(m_application->GetWindow());
	textBox->Initialize(p_position, p_dimension, "", m_application->GetRender(), Shape::Rectangle, p_isGUI, false, "");
	textBox->InitTextBox(p_id, p_color, p_isGUI);
	m_application->m_entityToAdd.push_back(textBox);
	AddEntity(textBox);

	return textBox;
}

Particles* Scene::CreateParticle(const std::string& m_texturePath, const std::string& p_computeShaderName, Vector2f p_spawnerPos){
	SDL_Texture* texture = AssetManager::Get()->GetTexture(m_texturePath);
	Particles* particles = new Particles(p_computeShaderName, Application::Get()->GetWindow(), texture, p_spawnerPos);
	m_particles.push_back(particles);
	return particles;
}

int Scene::GetWindowWidth() const
{
	return m_application->m_window.GetWindowSize().GetX();
}

int Scene::GetWindowHeight() const
{
	return m_application->m_window.GetWindowSize().GetY();
}

Vector2i Scene::GetWindowSize() const
{
	return m_application->m_window.GetWindowSize();
}

Vector2f Scene::GetWindowCenter() const
{
	return (Vector2f)m_application->m_window.GetWindowSize()/2;
}
	
float Scene::GetDeltaTime() const
{
	return m_application->m_deltaTime;
}

void Scene::ChangeisRunning(bool p_bool)
{
	m_application->m_isRunning = p_bool;
}

void Scene::ChangeAutoFPS()
{
	if (m_application->m_autoFPS == false)
	{
		m_application->m_autoFPS = true;
		std::cerr << "Auto FPS enabled." << std::endl;
	}
	else
	{
		m_application->m_autoFPS = false;
		std::cerr << "Auto FPS disabled." << std::endl;
	}
}

bool Scene::GetAutoFPS()
{
	return m_application->m_autoFPS;
}

void Scene::SetFPS(int p_newFPS)
{
	m_application->SetFPS(p_newFPS);
}

int Scene::GetCurrentFPS() const
{
	return static_cast<int>(m_application->m_currentFPS);
}

Application* Scene::GetApplication() const
{
	return m_application;
}

Camera* Scene::GetCamera() const
{
	return m_application->m_camera;
}

void Scene::SetIsVisible(bool p_visible)
{
	if (p_visible == true)
	{
		m_isVisible = true;

		for (Entity* entity : m_entities)
		{
			entity->SetIsVisible(true);
		}
		for (Geometry* geo : m_geometries)
		{
			geo->SetIsVisible(true);
		}
	}
	else
	{
		m_isVisible = false;
		for (Entity* entity : m_entities)
		{
			entity->SetIsVisible(false);
		}
		for (Geometry* geo : m_geometries)
		{
			geo->SetIsVisible(false);
		}
	}
}

void Scene::AddEntity(Entity* p_entity)
{
	m_entities.push_back(p_entity);
}

void Scene::RemoveEntity(Entity* p_entity)
{
	if (m_entities.empty())
	{
		return;
	}
	auto it = std::find(m_entities.begin(), m_entities.end(), p_entity);

	if (it == m_entities.end())
	{
		return;
	}

	it.operator*()->SetToDestroy(true);
	m_entities.erase(it);
}

void Scene::RemoveGeometry(const std::string& p_id)
{
	Debug* d = Debug::Get();

	Geometry* geo = d->GetGeometry<Geometry>(p_id);

	if (!geo)
		return;

	auto it = std::find(m_geometries.begin(), m_geometries.end(), geo);

	if (it == m_geometries.end())
	{
		return;
	}

	m_geometries.erase(it);

	d->Destroy(p_id);
}

void Scene::CreatePoint(const std::string& p_id, Vector2f p_position, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	Geometry* geo = Debug::Get()->CreatePoint(p_id, p_position, p_color, p_isGUI, p_clampToScreen);
	m_geometries.push_back(geo);
}
void Scene::CreateLine(const std::string& p_id, Vector2f p_start, Vector2f p_end, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	Geometry* geo = Debug::Get()->CreateLine(p_id, p_start, p_end, p_color, p_isGUI, p_clampToScreen);
	m_geometries.push_back(geo);
}
void Scene::CreateRect(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	Geometry* geo = Debug::Get()->CreateRect(p_id, p_position, p_dimension, p_color, p_isGUI, p_clampToScreen);
	m_geometries.push_back(geo);
}
void Scene::CreateFillRect(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	Geometry* geo = Debug::Get()->CreateFillRect(p_id, p_position, p_dimension, p_color, p_isGUI, p_clampToScreen);
	m_geometries.push_back(geo);
}
void Scene::CreateCircle(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	Geometry* geo = Debug::Get()->CreateCircle(p_id, p_position, p_dimension, p_color, p_isGUI, p_clampToScreen);
	m_geometries.push_back(geo);
}
void Scene::CreateFillCircle(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	Geometry* geo = Debug::Get()->CreateFillCircle(p_id, p_position, p_dimension, p_color, p_isGUI, p_clampToScreen);
	m_geometries.push_back(geo);
}
void Scene::CreateText(const std::string& p_id, Vector2f p_position, const std::string& p_text, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	Geometry* geo = Debug::Get()->CreateText(p_id, p_position, p_text, p_color, p_isGUI, p_clampToScreen);
	m_geometries.push_back(geo);
}

Entity* Scene::GetEntityByTag(int p_tag)
{
	for (Entity* entity : m_entities)
	{
		if (entity->GetTag() == p_tag)
		{
			return entity;
		}
	}
}

std::vector<Entity*> Scene::GetEntitiesByTag(int p_tag)
{
	std::vector<Entity*> entities;
	for (Entity* entity : m_entities)
	{
		if (entity->GetTag() == p_tag)
		{
			entities.push_back(entity);
		}
	}
	return entities;
}

void Scene::DestroyEntityByTag(int p_tag)
{
	GetEntityByTag(p_tag)->Destroy();
}

void Scene::DestroyEntitiesByTag(int p_tag)
{
	for (Entity* ent : GetEntitiesByTag(p_tag))
	{
		ent->Destroy();
	}
}

