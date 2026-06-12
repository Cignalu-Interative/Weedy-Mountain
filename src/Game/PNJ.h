#pragma once
#include "2DPendentEngine/Entity.h"
#include "Dialogue.h"

class PNJ : public Entity
{
private : 
	bool m_isInPlayerZone = false;

	bool m_isTalking = false;
	bool m_isFinished = true;

	float m_interval = 0.1f;
	float m_timer = m_interval;

	float m_textX;
	float m_textY;

	int m_index = 0;

	std::string m_toSay = "";
	std::string m_initial = "";

	std::vector<Dialogue::Choice> m_choices;

	std::string m_dialogPath = "";
public : 

	void OnInitialize() override;
	void OnEvent() override;
	void OnUpdate() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

	void SetDialogPath(std::string p_path) { m_dialogPath = p_path; }

	void Talk(const std::string& p_say);
	void Talk(int p_dialogueID, int p_choiceID);
	void EndTalk();
	void UpdateDialog();
	bool IsTalking() { return m_isTalking; }
	bool IsInPlayerZone() { return m_isInPlayerZone; }
	bool IsFinished() { return m_isFinished; }

	std::vector<Dialogue::Choice> GetChoices() { return m_choices; }
};

