#include "pch.h"
#include "QuestManager.h"

QuestManager* QuestManager::GetInstance()
{
	static QuestManager instance;
	return &instance;
}

QuestManager::QuestManager()
{


}

QuestManager::~QuestManager()
{


}

void QuestManager::AddQuest(Quest* p_quest)
{
	for (Quest* quest : m_questsVector)
	{
		if (quest->GetName() == p_quest->GetName())
		{
			SDL_Log("QuestManager::AddQuest/Quest with name '%s' already exists", p_quest->GetName().c_str());
			return;
		}
	}
	m_quests[p_quest->GetName().c_str()] = m_questsVector.size();
	m_questsVector.push_back(p_quest);
}

void QuestManager::RemoveQuest(const std::string& p_name)
{
	if (m_questsVector.empty())
	{
		return;
	}
	auto it = m_quests.find(p_name);

	if (it == m_quests.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_questsVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_questsVector[indexToDelete], m_questsVector[lastIndex]);
		m_quests[m_questsVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_questsVector[lastIndex];
	m_questsVector[lastIndex] = nullptr;
	m_questsVector.pop_back();
	m_quests.erase(it);
}

void QuestManager::RemoveAllQuests()
{
	for (auto& quest : m_questsVector)
	{
		delete quest;
	}
	m_questsVector.clear();
	m_quests.clear();
}

Quest* QuestManager::GetQuest(const std::string& p_name) const
{
	auto it = m_quests.find(p_name);
	if (it == m_quests.end())
	{
		SDL_Log("Game/QuestManager.cpp/GetQuest/find/quest with id '%s' not found", p_name.c_str());
		return nullptr;
	}

	return m_questsVector[it->second];
}

