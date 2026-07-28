#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Quest.h"
#include "2DPendentLib/Vector2.h"
#include "2DPendentEngine/Entity.h"

class QuestManager
{
private:

	std::unordered_map<std::string, size_t> m_questsNotStarted;
	std::vector<Quest*> m_questsNotStartedVector;

	std::unordered_map<std::string, size_t> m_questsInProcess;
	std::vector<Quest*> m_questsInProcessVector;

	std::unordered_map<std::string, size_t> m_questsFinished;
	std::vector<Quest*> m_questsFinishedVector;

	std::vector<Quest*> m_allQuests;

	Scene* m_scene = nullptr;

	void RemoveQuestFromNotStarted(Quest* p_quest);
	void RemoveQuestFromInProcess(Quest* p_quest);
	void RemoveQuestFromFinished(Quest* p_quest);

public:

	static QuestManager* Get();

	void SetScene(Scene* p_scene) { m_scene = p_scene; }
	Scene* GetScene() const { return m_scene; }

	QuestManager();
	~QuestManager();	

	void CreateQuest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner);
	void RemoveAllQuests();
	
	void DeleteQuestOnNotStarted(const std::string& p_name);
	void DeleteQuestOnInProcess(const std::string& p_name);
	void DeleteQuestOnFinished(const std::string& p_name);

	void AddQuestToNotStarted(Quest* p_quest);
	void AddQuestToInProcess(Quest* p_quest);
	void AddQuestToFinished(Quest* p_quest);

	Quest* GetQuestOnNotStarted(const std::string& p_name) const;
	Quest* GetQuestOnInProcess(const std::string& p_name) const;
	Quest* GetQuestOnFinished(const std::string& p_name) const;

	std::vector<Quest*>& GetQuestsNotStarted() { return m_questsNotStartedVector; }
	std::vector<Quest*>& GetQuestsInProcess() { return m_questsInProcessVector; }
	std::vector<Quest*>& GetQuestsFinished() { return m_questsFinishedVector; }

	std::vector<Quest*>& GetAllQuests();

	void UpdateQuests();

	void DrawQuests();

	void CheckAddItem(const std::string& ID, int amount);

	void SetStateQuestNotStarted(const std::string& p_name);
	void SetStateQuestInProcess(const std::string& p_name);
	void SetStateQuestFinished(const std::string& p_name);

	void GiveReward(Quest* p_quest);

};

