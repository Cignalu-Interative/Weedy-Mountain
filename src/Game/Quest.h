#pragma once
#include <string>
#include <vector>
#include "2DPendentLib/Vector2.h"
#include "2DPendentEngine/Entity.h"
#include "2DPendentLib/Text.h"
#include "2DPendentLib/Drawable.h"
#include "2DPendentLib/json.hpp"
#include "QuestObjective.h"
#include "ItemData.h"
#include "ItemDataBase.h"

enum class QuestState
{
	Blocked,
	NotStarted,
	InProgress,
	Finished
};

struct QuestReward
{
	ItemData* s_itemData = nullptr;
	int s_amount = 0;

	void SetItemData(ItemData* p_itemData) { s_itemData = p_itemData; }
	ItemData* GetItemData() const { return s_itemData; }

	void SetAmount(int p_amount) { s_amount = p_amount; }
	int GetAmount() const { return s_amount; }
};

class Quest : public Drawable
{
private:

	std::string m_name;
	Entity* m_questOwner = nullptr;

	QuestState m_state = QuestState::Blocked;

	std::vector<QuestObjective*> m_objectives;
	std::vector<QuestReward*> m_rewards;

	Entity* m_zone = nullptr;
	Entity* m_icone = nullptr;

	Vector2i m_dimSpriteObjective = { 32, 32 };

	Scene* m_scene = nullptr;

	bool m_rewardClaimed = false;

	std::string m_historyDescription;

	std::vector<Text*> m_progressTexts;
	std::vector<Text*> m_requiredTexts;

public:

	Quest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner, Scene* p_scene);
	~Quest();

	void SetName(const std::string& p_name) { m_name = p_name; }
	const std::string& GetName() const { return m_name; }

	void SetQuestOwner(Entity* p_owner) { m_questOwner = p_owner; }
	Entity* GetQuestOwner() const { return m_questOwner; }

	void SetState(QuestState p_state) { m_state = p_state; }
	QuestState GetState() const { return m_state; }

	bool IsBlocked() const { return m_state == QuestState::Blocked; }
	bool IsNotStarted() const { return m_state == QuestState::NotStarted; }
	bool IsInProcess() const { return m_state == QuestState::InProgress; }
	bool IsFinished() const { return m_state == QuestState::Finished; }

	void AddObjective(QuestObjective* p_objective);
	QuestObjective* GetObjective(size_t p_index) const;
	size_t GetObjectiveCount() const { return m_objectives.size(); }

	void AddReward(ItemData* p_itemData, int p_amount);
	QuestReward* GetReward(size_t p_index) const;
	size_t GetRewardsCount() const { return m_rewards.size(); }

	void OnEvent(const QuestEvent& p_event);

	bool AreAllObjectivesCompleted() const;

	void RefreshVisuals();
	void ResizeGrid();

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

	void SetScene(Scene* p_scene) { m_scene = p_scene; }
	Scene* GetScene() const { return m_scene; }

	void SetHistoryDescription(const std::string& p_description) { m_historyDescription = p_description; }
	const std::string& GetHistoryDescription() const { return m_historyDescription; }

	std::string GetRewardDescription() const;
	std::string GetObjectivesDescription() const;

	void SetVisible(bool p_visible);
	bool GetVisible() const { return (m_zone->GetIsVisible() || m_icone->GetIsVisible()); }

	bool IsRewardClaimed() const { return m_rewardClaimed; }
	void SetRewardClaimed(bool p_claimed) { m_rewardClaimed = p_claimed; }

	Entity* GetZone() const { return m_zone; }

	void Save(nlohmann::json& p_out) const;
	void Load(const nlohmann::json& p_in);
};