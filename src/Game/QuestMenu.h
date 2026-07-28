#pragma once
#include <vector>
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

public:

	QuestMenu(int p_slotCount = 6);
	~QuestMenu();

	void OnInitialize() override;

	int GetSlotCount() const { return (int)m_slots.size(); }

	void SyncQuests(const std::vector<Quest*>& p_questVector);

	void SetSelectedQuest(Quest* p_quest) { m_selectedQuest = p_quest; }
	Quest* GetSelectedQuest() const { return m_selectedQuest; }

	void SetVisible(bool p_visible);

	void Open();
	void Close();
	void Toggle();
};

