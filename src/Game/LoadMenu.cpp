#include "pch.h"
#include "LoadMenu.h"
#include "2DPendentEngine/Scene.h"
#include "2DPendentEngine/Application.h"

void LoadMenu::InitializeLoadMenu(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, bool p_animation, const std::string& p_jsonAnimation, Scene* p_scene, bool p_isGUI)
{
	Window* w = Application::Get()->GetWindow();
	// Create background entity
	m_background = p_scene->CreateEntity<Entity>((Vector2f)w->GetHalfWindowSize(), w->GetWindowSize(), "Background_Black", Shape::Rectangle, p_isGUI, false, "");
	m_background->SetDrawLayer(INT_MAX - 1);

	// Create loading animation entity
	m_loadingAnimation = p_scene->CreateEntity<Entity>(p_position, p_dimension, p_path, Shape::Rectangle, p_isGUI, p_animation, p_jsonAnimation);
}
