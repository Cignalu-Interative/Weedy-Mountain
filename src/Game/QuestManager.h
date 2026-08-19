#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Quest.h"
#include "QuestEvent.h"
#include "Savable.h"
#include "2DPendentLib/Vector2.h"
#include "2DPendentEngine/Entity.h"

class QuestManager : public Savable
{
private:

	std::unordered_map<std::string, Quest*> m_quests;
	std::vector<Quest*> m_allQuestsCache;

	Scene* m_scene = nullptr;

	Quest* m_activeQuest = nullptr;

	QuestManager();

public:

	static QuestManager* Get();
	~QuestManager();

	void SetScene(Scene* p_scene) { m_scene = p_scene; }
	Scene* GetScene() const { return m_scene; }

	Quest* CreateQuest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner);

	void RemoveAllQuests();
	void DeleteQuest(const std::string& p_name);

	Quest* GetQuest(const std::string& p_name) const;

	void UnlockQuest(const std::string& p_name) const;

	std::vector<Quest*> GetQuestsByState(QuestState p_state) const;
	std::vector<Quest*>& GetAllQuests();

	void NotifyEvent(const QuestEvent& p_event);

	void SetQuestState(const std::string& p_name, QuestState p_state);

	void GiveReward(Quest* p_quest);

	void SetActiveQuest(const std::string& p_name);
	Quest* GetActiveQuest() const { return m_activeQuest; }
	void RemoveActiveQuest(Quest* p_quest);

	void UpdateQuests();

	std::string GetSaveID() const override { return "questManager"; }
	void Save(nlohmann::json& p_out) override;
	void Load(const nlohmann::json& p_in) override;
};