#pragma once
#include <vector>
#include "2DPendentEngine/Entity.h"
#include "QuestMenuSlot.h"

class QuestMenu : public Entity
{
private:
	
	std::vector<QuestMenuSlot*> m_slots;

public:

	QuestMenu(int p_slotCount = 6);
	~QuestMenu();

	void OnInitialize() override;

	int GetSlotCount() const { return (int)m_slots.size(); }


};

