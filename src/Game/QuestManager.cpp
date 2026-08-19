#include "pch.h"
#include "QuestManager.h"
#include "ItemDataBase.h"
#include "GameData.h"
#include "SaveManager.h"

QuestManager* QuestManager::Get()
{
	static QuestManager instance;
	return &instance;
}

QuestManager::QuestManager()
{
	SaveManager::Get().RegisterProvider(this);
}

QuestManager::~QuestManager()
{
	SaveManager::Get().UnregisterProvider(this);
	RemoveAllQuests();
}

Quest* QuestManager::CreateQuest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner)
{
	if (GetQuest(p_name) != nullptr)
	{
		SDL_Log("QuestManager::CreateQuest/Quest with name '%s' already exists", p_name.c_str());
		return GetQuest(p_name);
	}

	Quest* quest = new Quest(p_name, p_position, p_dimension, p_owner, m_scene);
	quest->GetZone()->GetCollider()->SetShow(false);
	quest->SetVisible(false);

	m_quests[p_name] = quest;
	return quest;
}

void QuestManager::RemoveAllQuests()
{
	for (auto& [name, quest] : m_quests)
	{
		if (quest)
			delete quest;
	}

	m_quests.clear();
	m_activeQuest = nullptr;
}

void QuestManager::DeleteQuest(const std::string& p_name)
{
	auto it = m_quests.find(p_name);

	if (it == m_quests.end())
		return;

	if (m_activeQuest == it->second)
		m_activeQuest = nullptr;

	delete it->second;
	m_quests.erase(it);
}

Quest* QuestManager::GetQuest(const std::string& p_name) const
{
	auto it = m_quests.find(p_name);

	if (it == m_quests.end())
		return nullptr;

	return it->second;
}

void QuestManager::UnlockQuest(const std::string& p_name) const
{
	auto it = m_quests.find(p_name);

	if (it == m_quests.end())
		return;

	it->second->SetState(QuestState::NotStarted);
}

std::vector<Quest*> QuestManager::GetQuestsByState(QuestState p_state) const
{
	std::vector<Quest*> result;

	for (auto& [name, quest] : m_quests)
	{
		if (quest && quest->GetState() == p_state)
			result.push_back(quest);
	}

	return result;
}

std::vector<Quest*>& QuestManager::GetAllQuests()
{
	m_allQuestsCache.clear();
	m_allQuestsCache.reserve(m_quests.size());

	for (auto& [name, quest] : m_quests)
	{
		if (quest->IsBlocked())
			continue;

		m_allQuestsCache.push_back(quest);
	}

	return m_allQuestsCache;
}

void QuestManager::NotifyEvent(const QuestEvent& p_event)
{
	std::vector<Quest*> inProcessQuests = GetQuestsByState(QuestState::InProgress);

	for (Quest* quest : inProcessQuests)
	{
		quest->OnEvent(p_event);

		if (quest->AreAllObjectivesCompleted())
			SetQuestState(quest->GetName(), QuestState::Finished);
	}
}

void QuestManager::SetQuestState(const std::string& p_name, QuestState p_state)
{
	Quest* quest = GetQuest(p_name);
	if (quest == nullptr)
		return;

	QuestState previousState = quest->GetState();
	if (previousState == p_state)
		return;

	quest->SetState(p_state);

	if (p_state == QuestState::InProgress)
	{
		if (m_activeQuest == nullptr)
		{
			m_activeQuest = quest;
			quest->SetVisible(true);
		}
		else
		{
			quest->SetVisible(false);
		}
	}

	if (p_state == QuestState::Finished && previousState != QuestState::Finished)
	{
		GiveReward(quest);
		RemoveActiveQuest(quest);
	}
}

void QuestManager::GiveReward(Quest* p_quest)
{
	if (p_quest == nullptr)
		return;

	for (size_t i = 0; i < p_quest->GetRewardsCount(); ++i)
	{
		QuestReward* reward = p_quest->GetReward(i);
		if (reward && reward->GetItemData())
		{
			GameData::Get().GetPlayerData().m_playerInventory->AddItem(reward->GetItemData()->GetID(), reward->GetAmount(), false);
		}
	}
}

void QuestManager::SetActiveQuest(const std::string& p_name)
{
	Quest* quest = GetQuest(p_name);
	if (quest == nullptr || quest->GetState() != QuestState::InProgress)
		return;

	if (m_activeQuest)
		m_activeQuest->SetVisible(false);

	m_activeQuest = quest;
	m_activeQuest->SetVisible(true);
}

void QuestManager::RemoveActiveQuest(Quest* p_quest)
{
	if (m_activeQuest == nullptr || p_quest == nullptr)
		return;

	if (m_activeQuest->GetName() != p_quest->GetName())
		return;

	m_activeQuest->SetVisible(false);
	m_activeQuest = nullptr;
}

void QuestManager::UpdateQuests()
{
	if (m_activeQuest)
		m_activeQuest->RefreshVisuals();
}

void QuestManager::Save(nlohmann::json& p_out)
{
	nlohmann::json questsJson;

	for (auto& [name, quest] : m_quests)
	{
		nlohmann::json questJson;
		quest->Save(questJson);
		questsJson[name] = questJson;
	}

	p_out["quests"] = questsJson;
}

void QuestManager::Load(const nlohmann::json& p_in)
{
	if (p_in.contains("quests") == false)
		return;

	for (auto& [name, questJson] : p_in["quests"].items())
	{
		Quest* quest = GetQuest(name);
		if (quest)
			quest->Load(questJson);
	}
}