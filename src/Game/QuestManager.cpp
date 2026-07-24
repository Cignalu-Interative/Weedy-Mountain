#include "pch.h"
#include "QuestManager.h"
#include "ItemDataBase.h"
#include "GameData.h"

QuestManager* QuestManager::Get()
{
	static QuestManager instance;
	return &instance;
}

QuestManager::QuestManager()
{


}

QuestManager::~QuestManager()
{
	RemoveAllQuests();
}

void QuestManager::CreateQuest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner)
{
	Quest* qns = GetQuestOnNotStarted(p_name);
	Quest* qip = GetQuestOnInProcess(p_name);
	Quest* qf = GetQuestOnFinished(p_name);

	if (qns || qip || qf)
	{
		SDL_Log("QuestManager::AddQuest/Quest with name '%s' already exists", p_name.c_str());
		return;
	}

	Quest* p_quest = new Quest(p_name, p_position, p_dimension, p_owner, m_scene);

	AddQuestToNotStarted(p_quest);
}

void QuestManager::RemoveAllQuests()
{
	for (auto& quest : m_questsNotStartedVector)
	{
		delete quest;
	}
	m_questsNotStartedVector.clear();
	m_questsNotStarted.clear();

	for (auto& quest : m_questsInProcessVector)
	{
		delete quest;
	}
	m_questsInProcessVector.clear();
	m_questsInProcess.clear();

	for (auto& quest : m_questsFinishedVector)
	{
		delete quest;
	}
	m_questsFinishedVector.clear();
	m_questsFinished.clear();
}

void QuestManager::DeleteQuestOnNotStarted(const std::string& p_name)
{
	if (m_questsNotStartedVector.empty())
	{
		return;
	}
	auto it = m_questsNotStarted.find(p_name);

	if (it == m_questsNotStarted.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_questsNotStartedVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_questsNotStartedVector[indexToDelete], m_questsNotStartedVector[lastIndex]);
		m_questsNotStarted[m_questsNotStartedVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_questsNotStartedVector[lastIndex];
	m_questsNotStartedVector[lastIndex] = nullptr;
	m_questsNotStartedVector.pop_back();
	m_questsNotStarted.erase(it);
}

void QuestManager::DeleteQuestOnInProcess(const std::string& p_name)
{
	if (m_questsInProcessVector.empty())
	{
		return;
	}
	auto it = m_questsInProcess.find(p_name);

	if (it == m_questsInProcess.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_questsInProcessVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_questsInProcessVector[indexToDelete], m_questsInProcessVector[lastIndex]);
		m_questsInProcess[m_questsInProcessVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_questsInProcessVector[lastIndex];
	m_questsInProcessVector[lastIndex] = nullptr;
	m_questsInProcessVector.pop_back();
	m_questsInProcess.erase(it);
}

void QuestManager::DeleteQuestOnFinished(const std::string& p_name)
{
	if (m_questsFinishedVector.empty())
	{
		return;
	}
	auto it = m_questsFinished.find(p_name);

	if (it == m_questsFinished.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_questsFinishedVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_questsFinishedVector[indexToDelete], m_questsFinishedVector[lastIndex]);
		m_questsFinished[m_questsFinishedVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_questsFinishedVector[lastIndex];
	m_questsFinishedVector[lastIndex] = nullptr;
	m_questsFinishedVector.pop_back();
	m_questsFinished.erase(it);
}

void QuestManager::RemoveQuestFromNotStarted(Quest* p_quest)
{
	auto itNS = m_questsNotStarted.find(p_quest->GetName());
	if (itNS != m_questsNotStarted.end())
	{
		m_questsNotStarted.erase(itNS);
	}
	auto itNSV = std::find(m_questsNotStartedVector.begin(), m_questsNotStartedVector.end(), p_quest);
	if (itNSV != m_questsNotStartedVector.end())
	{
		m_questsNotStartedVector.erase(itNSV);
	}
}

void QuestManager::RemoveQuestFromInProcess(Quest* p_quest)
{
	auto itIP = m_questsInProcess.find(p_quest->GetName());
	if (itIP != m_questsInProcess.end())
	{
		m_questsInProcess.erase(itIP);
	}
	auto itIPV = std::find(m_questsInProcessVector.begin(), m_questsInProcessVector.end(), p_quest);
	if (itIPV != m_questsInProcessVector.end())
	{
		m_questsInProcessVector.erase(itIPV);
	}
}

void QuestManager::RemoveQuestFromFinished(Quest* p_quest)
{
	auto itNS = m_questsFinished.find(p_quest->GetName());
	if (itNS != m_questsFinished.end())
	{
		m_questsFinished.erase(itNS);
	}
	auto itNSV = std::find(m_questsFinishedVector.begin(), m_questsFinishedVector.end(), p_quest);
	if (itNSV != m_questsFinishedVector.end())
	{
		m_questsFinishedVector.erase(itNSV);
	}
}

void QuestManager::AddQuestToNotStarted(Quest* p_quest)
{
	if (GetQuestOnNotStarted(p_quest->GetName()))
	{
		SDL_Log("QuestManager::AddQuestToNotStarted/Quest with name '%s' already exists in Not Started", p_quest->GetName().c_str());
		return;
	}

	RemoveQuestFromInProcess(p_quest);
	RemoveQuestFromFinished(p_quest);

	m_questsNotStarted[p_quest->GetName()] = m_questsNotStartedVector.size();
	m_questsNotStartedVector.push_back(p_quest);
}

void QuestManager::AddQuestToInProcess(Quest* p_quest)
{
	if (GetQuestOnInProcess(p_quest->GetName()))
	{
		SDL_Log("QuestManager::AddQuestToInProcess/Quest with name '%s' already exists in In Process", p_quest->GetName().c_str());
		return;
	}

	RemoveQuestFromNotStarted(p_quest);
	RemoveQuestFromFinished(p_quest);

	m_questsInProcess[p_quest->GetName()] = m_questsInProcessVector.size();
	m_questsInProcessVector.push_back(p_quest);
}

void QuestManager::AddQuestToFinished(Quest* p_quest)
{
	if (GetQuestOnFinished(p_quest->GetName()))
	{
		SDL_Log("QuestManager::AddQuestToFinished/Quest with name '%s' already exists in Finished", p_quest->GetName().c_str());
		return;
	}

	RemoveQuestFromNotStarted(p_quest);
	RemoveQuestFromInProcess(p_quest);

	m_questsFinished[p_quest->GetName()] = m_questsFinishedVector.size();
	m_questsFinishedVector.push_back(p_quest);
}

Quest* QuestManager::GetQuestOnNotStarted(const std::string& p_name) const
{
	auto it = m_questsNotStarted.find(p_name);
	if (it == m_questsNotStarted.end())
	{
		SDL_Log("Game/QuestManager.cpp/GetQuest/find/quest with id '%s' not found", p_name.c_str());
		return nullptr;
	}

	return m_questsNotStartedVector[it->second];
}

Quest* QuestManager::GetQuestOnInProcess(const std::string& p_name) const
{
	auto it = m_questsInProcess.find(p_name);
	if (it == m_questsInProcess.end())
	{
		SDL_Log("Game/QuestManager.cpp/GetQuest/find/quest with id '%s' not found", p_name.c_str());
		return nullptr;
	}

	return m_questsInProcessVector[it->second];
}

Quest* QuestManager::GetQuestOnFinished(const std::string& p_name) const
{
	auto it = m_questsFinished.find(p_name);
	if (it == m_questsFinished.end())
	{
		SDL_Log("Game/QuestManager.cpp/GetQuest/find/quest with id '%s' not found", p_name.c_str());
		return nullptr;
	}

	return m_questsFinishedVector[it->second];
}

void QuestManager::UpdateQuests()
{
	for (Quest* quest : m_questsInProcessVector)
	{
		if (quest)
		{
			quest->UpdateQuest();
		}
	}
}

void QuestManager::DrawQuests()
{
	/*for (Quest* quest : m_questsVector)
	{
		if (quest)
		{
			quest->Draw(Application::Get()->GetRender(), Application::Get()->GetWindow(), Application::Get()->GetCamera());
		}
	}*/
}

void QuestManager::CheckAddItem(const std::string& ID, int amount)
{
	for (Quest* quest : m_questsInProcessVector)
	{
		for (size_t i = 0; i < quest->GetRequirementsCount(); ++i)
		{
			QuestRequirement* requirement = quest->GetRequirement(i);
			if (requirement->GetCheckType() == CheckQuest::AddItem)
			{
				if (ItemDataBase::Get()->GetItem(ID) == requirement->GetItemData())
				{
					requirement->AddHarvestedAmount(amount);
				}
			}
		}
	}
}

void QuestManager::SetStateQuestNotStarted(const std::string& p_name)
{
	if (Quest* quest = GetQuestOnInProcess(p_name))
	{
		quest->SetNotStarted();
		AddQuestToNotStarted(quest);
	}
	else if (Quest* quest = GetQuestOnFinished(p_name))
	{
		quest->SetNotStarted();
		AddQuestToNotStarted(quest);
	}
}

void QuestManager::SetStateQuestInProcess(const std::string& p_name)
{
	if (Quest* quest = GetQuestOnNotStarted(p_name))
	{
		quest->SetInProcess();
		AddQuestToInProcess(quest);
	}
	else if (Quest* quest = GetQuestOnFinished(p_name))
	{
		quest->SetInProcess();
		AddQuestToInProcess(quest);
	}
}

void QuestManager::SetStateQuestFinished(const std::string& p_name)
{
	if (Quest* quest = GetQuestOnNotStarted(p_name))
	{
		quest->SetFinished();
		AddQuestToFinished(quest);
		GiveReward(quest);
	}
	else if (Quest* quest = GetQuestOnInProcess(p_name))
	{
		quest->SetFinished();
		AddQuestToFinished(quest);
		GiveReward(quest);
	}
}

void QuestManager::GiveReward(Quest* p_quest)
{
	for (size_t i = 0; i < p_quest->GetRewardsCount(); ++i)
	{
		QuestReward* reward = p_quest->GetReward(i);
		if (reward && reward->GetItemData())
		{
			GameData::Get().GetPlayerData().m_playerInventory->AddItem(reward->GetItemData()->GetID(), reward->GetAmount(), false);
		}
	}
}

