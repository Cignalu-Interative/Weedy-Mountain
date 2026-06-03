#include "pch.h"
#include "SceneManager.h"
#include "Application.h"
#include "Scene.h"

SceneManager::SceneManager()
{
	m_scenes.reserve(12); // Réserver de la mémoire pour 12 scènes afin d'éviter les reallocations fréquentes lors de l'ajout de scènes
	m_scenesVector.reserve(12); // Réserver de la mémoire pour 12 scènes afin d'éviter les reallocations fréquentes lors de l'ajout de scènes
}

SceneManager::~SceneManager()
{
	ClearScenes();
}

SceneManager* SceneManager::Get()
{
	static SceneManager instance;
	return &instance;
}

//void SceneManager::ChangeCurrentScene()
//{
//	if (m_tempCurrentSceneID.empty())
//		return;
//
//	if (auto it = m_scenes.find(m_tempCurrentSceneID); it != m_scenes.end())
//	{
//		if (m_currentScene)
//		{
//			m_currentScene->OnDestroy();
//		}
//		m_currentScene = m_scenesVector[it->second];
//		m_currentScene->OnInitialize();
//	}
//	m_tempCurrentSceneID.clear();
//}

void SceneManager::RemoveScene(const std::string& p_id)
{
	if (m_scenesVector.empty())
	{
		return;
	}
	auto it = m_scenes.find(p_id);
	if (it == m_scenes.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_scenesVector.size() - 1;

	m_scenesVector[indexToDelete]->OnDestroy();

	if (indexToDelete != lastIndex)
	{
		std::swap(m_scenesVector[indexToDelete], m_scenesVector[lastIndex]);
		m_scenes[m_scenesVector[indexToDelete]->GetName()] = indexToDelete;
	}

	delete m_scenesVector[lastIndex];
	m_scenesVector[lastIndex] = nullptr;
	m_scenesVector.pop_back();
	m_scenes.erase(it);
}

void SceneManager::ClearScenes()
{
	for (auto& scene : m_scenesVector)
	{
		scene->OnDestroy();
		delete scene;
	}
	m_scenesVector.clear();
	m_scenes.clear();
}

//void SceneManager::UpdateCurrentScene()
//{
//	//m_currentScene->OnEvent();
//	m_currentScene->OnUpdate();
//}
//
//void SceneManager::SetCurrentScene(const std::string& p_id)
//{
//	m_tempCurrentSceneID = p_id;
//}

void SceneManager::PushScene(const std::string& p_id)
{
	Scene* scene = GetScene(p_id);
	if (scene == nullptr)
	{
		return;
	}

	m_scenesToUpdate[p_id] = m_scenesToUpdateVector.size();
	m_scenesToUpdateVector.push_back(scene);
	scene->Initialize();
}

void SceneManager::PopScene(const std::string& p_id)
{
	if (m_scenesToUpdateVector.empty())
	{
		return;
	}
	auto it = m_scenesToUpdate.find(p_id);

	if (it == m_scenesToUpdate.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_scenesToUpdateVector.size() - 1;

	m_scenesToUpdateVector[indexToDelete]->Destroy();

	if (indexToDelete != lastIndex)
	{
		std::swap(m_scenesToUpdateVector[indexToDelete], m_scenesToUpdateVector[lastIndex]);
		m_scenesToUpdate[m_scenesToUpdateVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_scenesToUpdateVector[lastIndex];
	m_scenesToUpdateVector[lastIndex] = nullptr;
	m_scenesToUpdateVector.pop_back();
	m_scenesToUpdate.erase(it);
}

void SceneManager::ClearScenesToUpdate()
{
	for (auto& scene : m_scenesToUpdateVector)
	{
		scene->Destroy();
		delete scene;
	}
	m_scenesToUpdateVector.clear();
	m_scenesToUpdate.clear();
}

void SceneManager::EventScenesToUpdate()
{
	for (Scene* scene : m_scenesToUpdateVector)
	{
		scene->Event();
	}
}

void SceneManager::UpdateScenesToUpdate()
{
	for (Scene* scene : m_scenesToUpdateVector)
	{
		scene->Update();
	}
}

bool SceneManager::IsSceneToUpdate() const
{
	if (m_scenesToUpdateVector.empty())
	{
		return false;
	}
	return true;
}

Scene* SceneManager::GetScene(const std::string& p_id) const
{
	auto it = m_scenes.find(p_id);
	if (it != m_scenes.end())
	{
		return m_scenesVector[it->second];
	}
	return nullptr;
}
