#include "pch.h"
#include "SceneManager.h"
#include "Application.h"
#include "Scene.h"
#include "2DPendentLib/Render.h"
#include "2DPendentLib/Window.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentLib/BatchRenderer2D.h"
#include "Entity.h"

SceneManager::SceneManager()
{
	m_scenes.reserve(12); // Réserver de la mémoire pour 12 scènes afin d'éviter les reallocations fréquentes lors de l'ajout de scènes
	m_scenesVector.reserve(12); // Réserver de la mémoire pour 12 scènes afin d'éviter les reallocations fréquentes lors de l'ajout de scènes

	m_scenesInStack.reserve(12); // Réserver de la mémoire pour 12 scènes afin d'éviter les reallocations fréquentes lors de l'ajout de scènes
	m_scenesInStackVector.reserve(12); // Réserver de la mémoire pour 12 scènes afin d'éviter les reallocations fréquentes lors de l'ajout de scènes

	m_entToDraw.reserve(4000); // Réserver de la mémoire pour 1000 entités afin d'éviter les reallocations fréquentes lors de l'ajout d'entités
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
	auto it = std::find(m_scenesInStackVector.begin(), m_scenesInStackVector.end(), scene);
	if (it != m_scenesInStackVector.end())
	{
		std::rotate(it, it + 1, m_scenesInStackVector.end());
	}
	else
	{
		m_scenesInStack[p_id] = m_scenesInStackVector.size();
		m_scenesInStackVector.push_back(scene);
	}

	if (m_scenesInStackVector.empty() == false)
	{
		for (Scene* sceneInStack : m_scenesInStackVector)
		{
			if (sceneInStack == scene)
			{
				continue;
			}
			sceneInStack->Sleep();
		}
	}
	scene->Wake();
	scene->Initialize();
}

void SceneManager::PopScene(const std::string& p_id)
{
	if (m_scenesInStackVector.empty())
	{
		return;
	}
	auto it = m_scenesInStack.find(p_id);

	if (it == m_scenesInStack.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_scenesInStackVector.size() - 1;

	m_scenesInStackVector[indexToDelete]->Destroy();

	if (indexToDelete != lastIndex)
	{
		std::swap(m_scenesInStackVector[indexToDelete], m_scenesInStackVector[lastIndex]);
		m_scenesInStack[m_scenesInStackVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_scenesInStackVector[lastIndex];
	m_scenesInStackVector[lastIndex] = nullptr;
	m_scenesInStackVector.pop_back();
	m_scenesInStack.erase(it);
}

void SceneManager::ClearScenesInStack()
{
	for (auto& scene : m_scenesInStackVector)
	{
		scene->Destroy();
		//delete scene;
	}
	m_scenesInStackVector.clear();
	m_scenesInStack.clear();
}

void SceneManager::EventScenesInStack()
{
	for (Scene* scene : m_scenesInStackVector)
	{
		scene->Event();
	}
}

void SceneManager::UpdateScenesInStack()
{
	for (Scene* scene : m_scenesInStackVector)
	{
		scene->Update();
	}
}

void SceneManager::DrawScenesInStack(Render* f_render, Window* f_window, Camera* p_camera)
{
	std::list<Entity*> tempEntity;
	for (Scene* scene : m_scenesInStackVector)
	{
		if (scene->Draw(f_render, f_window, p_camera) == false)
		{
			continue;
		}
		tempEntity = scene->GetEntities();
		for (Entity* entity : tempEntity)
		{
			m_entToDraw.push_back(entity);
		}
		//scene->Draw(f_render, f_window, p_camera);
	}

	std::sort(m_entToDraw.begin(), m_entToDraw.end(), [](const Entity* a, const Entity* b)
	{
		return a->GetDrawLayer() < b->GetDrawLayer();
	});

	/*entToDraw.sort([](const Entity* a, const Entity* b)
	{
		return a->GetDrawLayer() < b->GetDrawLayer();
	});*/

	for (Entity* entity : m_entToDraw)
	{
		entity->Draw(f_render, f_window);
	}

	m_entToDraw.clear();

	const std::vector<Data>& batches = BatchRenderer2D::Get()->Flush();
	for (const auto& batch : batches)
	{
		if (batch.s_vertices.empty())
			continue; // sécurité : un slot recyclé mais pas réutilisé cette frame

		if (SDL_RenderGeometry(f_render->GetRenderer(), batch.s_texture,
			batch.s_vertices.data(), static_cast<int>(batch.s_vertices.size()),
			batch.s_indices.data(), static_cast<int>(batch.s_indices.size())) == false)
		{
			SDL_Log("Engine/SceneManager.cpp/DrawScenesInStack/SDL_RenderGeometry/cela na pas fonctionné : %s", SDL_GetError());
		}
	}

}

bool SceneManager::IsSceneInStack() const
{
	if (m_scenesInStackVector.empty())
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
