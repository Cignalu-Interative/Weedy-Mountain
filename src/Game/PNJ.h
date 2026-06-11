#pragma once
#include "2DPendentEngine/Entity.h"

class PNJ : public Entity
{
private : 
	bool m_isInPlayerZone = false;

	bool m_isTalking = false;
	bool m_isFinished = true;

	float m_interval = 0.2f;
	float m_timer = m_interval;
	float m_rectX = 0;
	float m_rectY = 0;
	float m_rectWidth = 0;
	float m_rectHeight = 0;
	int m_index = 0;

	Entity* m_dialogBox = nullptr;

	std::string m_toSay = "";
	std::string m_initial = "";
public : 

	void OnInitialize() override;
	void OnEvent() override;
	void OnUpdate() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

	void Talk(const std::string& p_say);
	void EndTalk();
	void UpdateDialog();
	bool IsTalking() { return m_isTalking; }
};

