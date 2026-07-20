#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Quest.h"

class QuestManager
{
private:

	std::unordered_map<std::string, size_t> m_quests;

	std::vector<Quest*> m_questsVector;


public:

	static QuestManager* GetInstance();

	QuestManager();
	~QuestManager();	

	void AddQuest(Quest* p_quest);

	void RemoveQuest(const std::string& p_name);

	void RemoveAllQuests();

	Quest* GetQuest(const std::string& p_name) const;

};

