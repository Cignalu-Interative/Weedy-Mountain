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

void SceneManager::ChangeCurrentScene()
{
	if (m_tempCurrentSceneID.empty())
		return;

	if (auto it = m_scenes.find(m_tempCurrentSceneID); it != m_scenes.end())
	{
		if (m_currentScene)
		{
			m_currentScene->OnDestroy();
		}
		m_currentScene = m_scenesVector[it->second];
		m_currentScene->OnInitialize();
	}
	m_tempCurrentSceneID.clear();
}

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

void SceneManager::UpdateCurrentScene()
{
	//m_currentScene->OnEvent();
	m_currentScene->OnUpdate();
}

void SceneManager::SetCurrentScene(const std::string& p_id)
{
	m_tempCurrentSceneID = p_id;
}