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
			m_dialoguesIdsVector.push_back(txtName);
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
	m_dialoguesIdsVector.push_back(txtName);
}

std::string* DialogueManager::GetDialogue(const std::string& p_name)
{
	return nullptr;
}

//void AssetManager::AddTextureByPath(const std::string& p_path)
//{
//	std::string autoId = Utils::GetNameFromPath(p_path);
//	auto it = m_texturesMap.find(autoId);
//	if (it != m_texturesMap.end())
//	{
//		SDL_Log("Lib/AssetManager.cpp/AddTextureByPath/GetNameFromPath/Texture already loaded with ID : %s", autoId.c_str());
//		return;
//	}
//	SDL_Texture* texture = IMG_LoadTexture(m_renderer, p_path.c_str());
//	if (texture == nullptr)
//	{
//		SDL_Log("Lib/AssetManager.cpp/AddTextureByPath/IMG_LoadTexture/failed to load texture: %s", SDL_GetError());
//		return;
//	}
//	m_texturesMap[autoId] = m_texturesVector.size();
//	m_texturesVector.push_back(texture);
//	m_textureIdsVector.push_back(autoId);
//}