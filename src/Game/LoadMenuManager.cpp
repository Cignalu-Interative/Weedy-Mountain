#include "pch.h"
#include "LoadMenuManager.h"
#include "2DPendentEngine/Application.h"
#include "2DPendentEngine/Scene.h"
#include "2DPendentLib/Utils.h"

LoadMenuManager* LoadMenuManager::Get()
{
	static LoadMenuManager instance;
	return &instance;
}

void LoadMenuManager::CreateLoadMenu(Vector2f p_position, Vector2i p_dimension, const std::string& p_id, bool p_animation, const std::string& p_jsonAnimation, bool p_isGUI)
{
	if (m_loadMenus.find(p_id) == m_loadMenus.end())
	{
		LoadMenu* loadMenu = new LoadMenu();
		loadMenu->InitializeLoadMenu(p_position, p_dimension, p_id, p_animation, p_jsonAnimation, m_scene, p_isGUI);
		loadMenu->SetName(p_id);
		m_loadMenus[p_id] = m_loadMenuVector.size();
		m_loadMenuVector.push_back(loadMenu);
		SDL_Log("Game/LoadMenuManager.cpp/CreateLoadMenu/GetNameFromPath/LoadMenu load with ID : %s", p_id.c_str());
	}
}

void LoadMenuManager::RemoveLoadMenu(const std::string& p_id)
{
	if (m_loadMenuVector.empty())
	{
		return;
	}
	auto it = m_loadMenus.find(p_id);
	if (it == m_loadMenus.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_loadMenuVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_loadMenuVector[indexToDelete], m_loadMenuVector[lastIndex]);
		m_loadMenus[m_loadMenuVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_loadMenuVector[lastIndex];
	m_loadMenuVector[lastIndex] = nullptr;
	m_loadMenuVector.pop_back();
	m_loadMenus.erase(it);
}

void LoadMenuManager::RemoveAllLoadMenus()
{
	for (auto& loadMenu : m_loadMenuVector)
	{
		delete loadMenu;
	}
	m_loadMenuVector.clear();
	m_loadMenus.clear();
}

LoadMenu* LoadMenuManager::GetLoadMenu(const std::string& p_id) const
{
	auto it = m_loadMenus.find(p_id);
	if (it == m_loadMenus.end())
	{
		SDL_Log("Game/LoadMenuManager.cpp/GetLoadMenu/find/LoadMenu with id '%s' not found", p_id.c_str());
		return nullptr;
	}

	return m_loadMenuVector[it->second];
}
