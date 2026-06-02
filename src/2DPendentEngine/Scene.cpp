#include "pch.h"
#include "Scene.h"
#include "Application.h"
#include "Entity.h"
#include "2DPendentLib/Shape.h"
#include "SoundManager.h"
#include "AssetManager.h"
#include "Camera.h"

#include "Button.h"
#include "Slider.h"

Button* Scene::CreateButton(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_textID, const std::string& p_text, bool p_isGUI)
{
	if (m_application == nullptr)
	{
		return nullptr;
	}

	Button* button = new Button();

	button->SetScene(this);
	button->Initialize(p_position, p_dimention, p_spritePath, m_application->GetRender(), Shape::Rectangle, p_isGUI);
	button->InitText(p_textID, p_text);
	m_application->m_entityToAdd.push_back(button);

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
	slider->Initialize(p_position, p_dimention, p_spritePath, m_application->GetRender(), Shape::Rectangle, p_isGUI);
	slider->InitSlider(p_id, p_dimention.GetX(), p_dimention.GetY(), p_stateCount);
	m_application->m_entityToAdd.push_back(slider);

	return slider;
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
