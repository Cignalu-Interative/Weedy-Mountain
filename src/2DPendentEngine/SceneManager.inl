#pragma once

#include "SceneManager.h"
#include "Application.h"

//Fonction template pour ajouter une scène dans le SceneManager
template<typename T>
void SceneManager::AddScene(const std::string& p_id)
{
	static_assert(std::is_base_of_v<Scene, T>, "T must be derived from Scene");
	//RemoveScene(p_id);

	T* scene = new T();
	scene->SetApplication(Application::Get());
	scene->SetName(p_id);
	m_scenes[p_id] = m_scenesVector.size();
	m_scenesVector.push_back(scene);

	if (IsSceneInStack() == false)
	{
		/*m_tempCurrentSceneID = p_id;
		ChangeCurrentScene();*/

		PushScene(p_id);
	}
}