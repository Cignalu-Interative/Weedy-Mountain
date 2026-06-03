#pragma once

#include "Scene.h"
#include "Application.h"
#include "Entity.h"

//Fonction template pour créer une entité dans la scène (seul la Width compte pour le raduis du cercle)
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
	AddEntity(entity);

	return newEntity;
}