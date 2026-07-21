#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Quest.h"
#include "2DPendentLib/Vector2.h"
#include "Entity.h"

class QuestManager
{
private:

	std::unordered_map<std::string, size_t> m_quests;

	std::vector<Quest*> m_questsVector;


public:

	static QuestManager* Get();

	QuestManager();
	~QuestManager();	

	void AddQuest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner);

	void RemoveQuest(const std::string& p_name);

	void RemoveAllQuests();

	Quest* GetQuest(const std::string& p_name) const;

	void UpdateQuests();

	void DrawQuests();

};

