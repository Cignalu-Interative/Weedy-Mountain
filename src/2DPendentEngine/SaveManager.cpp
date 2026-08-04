#include "pch.h"
#include "SaveManager.h"
#include "Savable.h"

SaveManager& SaveManager::Get()
{
	static SaveManager Instance;
	return Instance;
}

void SaveManager::RegisterProvider(Savable* p_provider)
{
	if (p_provider == nullptr)
		return;

	auto it = std::find(m_providers.begin(), m_providers.end(), p_provider);
	if (it != m_providers.end())
		return;

	m_providers.push_back(p_provider);
}

void SaveManager::UnregisterProvider(Savable* p_provider)
{
	auto it = std::find(m_providers.begin(), m_providers.end(), p_provider);
	if (it != m_providers.end())
		m_providers.erase(it);
}

bool SaveManager::SaveToFile(const std::string& p_saveName)
{
	json root;

	for (Savable* provider : m_providers)
	{
		json section;
		provider->Save(section);
		root[provider->GetSaveID()] = section;
	}

	std::filesystem::create_directories(m_saveFolder);
	std::ofstream file(m_saveFolder + p_saveName + ".json");

	if (file.is_open() == false)
		return false;
	
	file << root.dump(4);

	return true;
}

bool SaveManager::LoadFromFile(const std::string& p_saveName)
{
	std::ifstream file(m_saveFolder + p_saveName + ".json");

	if (file.is_open() == false)
		return false;

	json root;
	try { root = json::parse(file); }
	catch (...) { return false; }

	for (Savable* provider : m_providers)
	{
		auto it = root.find(provider->GetSaveID());

		if (it == root.end())
			continue;

		provider->Load(*it);
	}

	return true;
}


