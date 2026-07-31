#pragma once
#include "2DPendentEngine/Entity.h"
#include "Dialogue.h"

class PNJ : public Entity
{
private : 

	Entity* m_dialogBox = nullptr;
	Scene* m_scene = nullptr;
	Button* m_button = nullptr;

	Vector2f m_pressSpacePos = {};
	Vector2i m_dialogBoxDim = {};
	Vector2f m_dialogBoxPos = {};

	bool m_isInPlayerZone = false;
	bool m_isTalking = false;
	bool m_isFinished = true;
	bool m_isPageFull = false;

	float m_interval = 0.015f;
	float m_timer = m_interval;
	float m_textX;
	float m_textY;

	float m_maxTextHeight = 0.f;

	int m_index = 0;

	std::vector<Dialogue::Choice> m_choices;
	std::vector<Button*> m_buttons;

	std::string m_initial = "";
	std::string m_toSay = "";
	std::string m_dialogPath = "";

	std::function<void()> m_onFinish; 
public : 

	void OnInitialize() override;
	void OnEvent() override;
	void OnUpdate() override;
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

	void SetDialogPath(std::string p_path) { m_dialogPath = p_path; }
	void SetOnFinishFunction(std::function<void()> p_onFinish) { m_onFinish = p_onFinish; }

	void Talk(const std::string& p_say);
	void Talk(int p_dialogueID);
	void EndTalk();
	void SkipTalk();
	void OnFinish();
	void UpdateDialog();

	bool IsTalking() { return m_isTalking; }
	bool IsInPlayerZone() { return m_isInPlayerZone; }
	bool IsFinished() { return m_isFinished; }

	std::vector<Dialogue::Choice> GetChoices() { return m_choices; }
};

