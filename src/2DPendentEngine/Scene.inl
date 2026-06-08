#pragma once

#include "Scene.h"
#include "Application.h"
#include "Entity.h"
#include "2DPendentLib/Geometry.h"
#include "Button.h"

//Fonction template pour cr�er une entit� dans la sc�ne (seul la Width compte pour le raduis du cercle)
template<typename T>
T* Scene::CreateEntity(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Shape p_shape, bool p_isGUI)
{
	//bool pour verifier si la class T est une entity si oui value = true
	static_assert(std::is_base_of_v<Entity, T>, "T must be derived from Entity");

	if (m_application == nullptr)
	{
		return nullptr;
	}
	
	T* newEntity = new T();

	Entity* entity = newEntity;
	entity->SetScene(this);
	entity->Initialize(p_position, p_dimension, p_path, m_application->GetRender(), p_shape, p_isGUI);
	m_application->m_entityToAdd.push_back(newEntity);
	AddEntity(newEntity);

	return newEntity;
}

template<typename T>
T* Scene::GetGeometry(const std::string& p_id)
{
	for (Geometry* geometry : m_geometries)
	{
		if (geometry->GetId() == p_id)
		{
			return dynamic_cast<T*>(geometry);
		}
	}
}
template<typename T>
T* Scene::CreateButton(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_textID, const std::string& p_text, bool p_isGUI)
{
	static_assert(std::is_base_of_v<Button, T>, "T must be derived from Button");

	if (m_application == nullptr)
	{
		return nullptr;
	}

	Button* button = new T();

	button->SetScene(this);
	button->Initialize(p_position, p_dimention, p_spritePath, m_application->GetRender(), Shape::Rectangle, p_isGUI);
	button->InitText(p_textID, p_text);
	m_application->m_entityToAdd.push_back(button);

	return button;
}
