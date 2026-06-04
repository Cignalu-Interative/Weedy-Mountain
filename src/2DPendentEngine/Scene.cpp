#include "pch.h"
#include "Scene.h"
#include "Application.h"
#include "Entity.h"
#include "2DPendentLib/Shape.h"
#include "SoundManager.h"
#include "AssetManager.h"
#include "Camera.h"


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
	OnWake();
}

void Scene::Destroy()
{
	m_isInCache = false;
	OnDestroy();
}

int Scene::GetWindowWidth() const
{
	return m_application->m_window.GetWindowSize().GetX();
}

int Scene::GetWindowHeight() const
{
	return m_application->m_window.GetWindowSize().GetY();
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
	switch (p_visible)
	{
	case true:

		m_isVisible = true;

		for (Entity* entity : m_entities)
		{
			entity->SetIsVisible(true);
		}
		for (Point point : Debug::Get()->GetAllPoints())
		{
			point.SetIsVisible(true);
		}
		for (Line line : Debug::Get()->GetAllLines())
		{
			line.SetIsVisible(true);
		}
		for (Rectangle rect : Debug::Get()->GetAllRects())
		{
			rect.SetIsVisible(true);
		}
		for (Rectangle fillRect : Debug::Get()->GetAllFillRects())
		{
			fillRect.SetIsVisible(true);
		}
		for (Circle circle : Debug::Get()->GetAllCircles())
		{
			circle.SetIsVisible(true);
		}
		for (Circle& fillCircle : Debug::Get()->GetAllFillCircles())
		{
			fillCircle.SetIsVisible(true);
		}
		for (Text* text : Debug::Get()->GetAllTexts())
		{
			text->SetIsVisible(true);
		}

		break;
	case false:

		m_isVisible = false;
		for (Entity* entity : m_entities)
		{
			entity->SetIsVisible(false);
		}
		for (Point point : Debug::Get()->GetAllPoints())
		{
			point.SetIsVisible(false);
		}
		for (Line line : Debug::Get()->GetAllLines())
		{
			line.SetIsVisible(false);
		}
		for (Rectangle rect : Debug::Get()->GetAllRects())
		{
			rect.SetIsVisible(false);
		}
		for (Rectangle fillRect : Debug::Get()->GetAllFillRects())
		{
			fillRect.SetIsVisible(false);
		}
		for (Circle circle : Debug::Get()->GetAllCircles())
		{
			circle.SetIsVisible(false);
		}
		for (Circle fillCircle : Debug::Get()->GetAllFillCircles())
		{
			fillCircle.SetIsVisible(false);
		}
		for (Text* text : Debug::Get()->GetAllTexts())
		{
			text->SetIsVisible(false);
		}

		break;
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

	m_entities.erase(it);
}
