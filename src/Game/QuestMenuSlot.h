#pragma once
#include "2DPendentEngine/Button.h"

class QuestMenuSlot : public Button
{
private:

	int m_pagesquest = 0;


public:

	void SetPagesQuest(int p_pagesquest) { m_pagesquest = p_pagesquest; }
	void AddPagesQuest(int p_pagesquest) { m_pagesquest += p_pagesquest; }
	int GetPagesQuest() const { return m_pagesquest; }

};

