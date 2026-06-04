#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Application.h"
//#include "Collider.h"

//Fonction template pour obtenir la scène à laquelle l'entité appartient
template<typename T>
T* Entity::GetScene() const
{
	T* scene = dynamic_cast<T*>(GetScene());

	_ASSERT(scene != nullptr);

	return scene;
}

//Fonction template pour créer une entité dans la scène à laquelle l'entité appartient (seul la Width compte pour le raduis du cercle)
template<typename T>
T* Entity::CreateEntity(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Shape p_shape)
{
	return GetScene()->CreateEntity<T>(p_position, p_dimension, p_path, p_shape);
}
