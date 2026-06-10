#include "pch.h"
#include "DialogueManager.h"
#include "Dialogue.h"
#include "json.hpp"
#include "Utils.h"

DialogueManager* DialogueManager::Get()
{
	static DialogueManager instance;
	return &instance;
}

DialogueManager::DialogueManager()
{
}

DialogueManager::~DialogueManager()
{
	//m_dialoguesMap.clear();
}

void DialogueManager::AddDialogueByRepertory(const std::string& p_path)
{
	std::ifstream file(p_path);
	if (!file.is_open())
	{
		std::cout << "Engine/DialogueManager.cpp/AddDialogueByRepertory/failed to open file: %s" << p_path.c_str() << std::endl;
		return;
	}

	nlohmann::ordered_json dialogues_json;
	file >> dialogues_json;

	for (auto& [categorie, assets] : dialogues_json.items())
	{
		for (auto& dialogue : assets)
		{
			std::string path = dialogue["path"].get<std::string>();
			std::string txtName = Utils::GetNameFromPath(path);
			auto it = m_dialoguesMap.find(txtName);
			if (it != m_dialoguesMap.end())
			{
				std::cout << "Engine/DialogueManager.cpp/AddDialogue/GetNameFromPath/Dialogue already loaded with ID : %s" << txtName.c_str() << std::endl;
				continue;
			}

			Dialogue* d = new Dialogue();
			if (d->ParserDialogueFile(path) == false)
			{
				std::cout << "Engine/DialogueManager.cpp/AddDialogue/  ParserDialogueFile error" << std::endl;
				return;
			}

			m_dialoguesMap[txtName] = m_dialoguesMap.size();
			m_dialoguesVector.push_back(d);
		}
	}
}

void DialogueManager::AddDialogueByPath(const std::string& p_path)
{
	std::string txtName = Utils::GetNameFromPath(p_path);
	auto it = m_dialoguesMap.find(txtName);
	if (it != m_dialoguesMap.end())
	{
		SDL_Log("Engine/DialogueManager.cpp/AddDialogueByPath/GetNameFromPath/Texture already loaded with ID : %s", txtName.c_str());
		return;
	}

	Dialogue* d = new Dialogue();
	if (d->ParserDialogueFile(p_path) == false)
	{
		std::cout << "Engine/DialogueManager.cpp/AddDialogue/  ParserDialogueFile error" << std::endl;
		return;
	}

	m_dialoguesMap[txtName] = m_dialoguesMap.size();
	m_dialoguesVector.push_back(d);

}

std::string DialogueManager::GetDialogueById(const std::string& p_nameFile, int p_id)
{

	auto it = m_dialoguesMap.find(p_nameFile);
	if (it == m_dialoguesMap.end())
	{
		SDL_Log("Engine/DialogueManager.cpp/GetDialogueById/find/Dialogue with id '%s' not found", p_nameFile.c_str());
		return nullptr;
	}

	return m_dialoguesVector[m_dialoguesMap[p_nameFile]]->GetTextById(p_id)->GetText();

}


// TO DO a tester
void DialogueManager::DestroyDialogueById(const std::string& p_id)
{
	auto it = m_dialoguesMap.find(p_id);
	if (it == m_dialoguesMap.end())
	{
		std::cout<<"Engine/DialogueManager.cpp/DestroyDialogueById/find/Texture with id '%s' not found " << p_id.c_str()<<std::endl;
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_dialoguesVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_dialoguesVector[indexToDelete], m_dialoguesVector[lastIndex]);
		m_dialoguesVector[indexToDelete] = m_dialoguesVector[lastIndex];
	}

	delete m_dialoguesVector[lastIndex];
	m_dialoguesVector[lastIndex] = nullptr;
	m_dialoguesVector.pop_back();
	m_dialoguesMap.erase(it);
}
