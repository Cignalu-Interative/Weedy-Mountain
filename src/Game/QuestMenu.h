#pragma once
#include <vector>
#include <string>
#include "2DPendentEngine/Entity.h"
#include "QuestMenuSlot.h"
#include "Quest.h"

class QuestMenu : public Entity
{
private:
	
	std::vector<QuestMenuSlot*> m_slots;
	std::vector<Quest*> m_quests;
	Quest* m_selectedQuest;

	bool m_isOpen = false;

	int m_NBPages = 0;
	int m_currentPage = 0;

	Text* m_rewardText = nullptr;
	Text* m_requirementText = nullptr;

public:

	QuestMenu(int p_slotCount = 9);
	~QuestMenu();

	void OnInitialize() override;

	int GetSlotCount() const { return (int)m_slots.size(); }

	void SyncQuests(const std::vector<Quest*>* p_questVector);

	void SetSelectedQuest(Quest* p_quest) { m_selectedQuest = p_quest; }
	Quest* GetSelectedQuest() const { return m_selectedQuest; }

	void SetVisible(bool p_visible);

	void Open(std::vector<Quest*>* p_quest);
	void Close();
	void Toggle(std::vector<Quest*>* p_quests);

	void OnUpdate() override;

	QuestMenuSlot* GetSlot(const std::string& p_id);

	void DisplayQuest(QuestMenuSlot* p_quest);
	void DisplayPage(int p_page = 0);

	int GetNBPages() const { return m_NBPages; }
	int GetCurrentPage() const { return m_currentPage; }

	void FollowQuest(Quest* p_quest);
	void UnfollowQuest(Quest* p_quest);

	bool GetIsOpen() const { return m_isOpen; }

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });
};

