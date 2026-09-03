#pragma once
#include "Scene.h"

class CreditScene : public Scene
{
private:

	Entity* m_background;

	Text* m_text;

	float m_timer = 0.f;
	float m_timerGoal = 20.f;

	void OnInitialize() override;
	void OnEvent() override {};
	void OnUpdate() override;
	void OnSleep() override;
	void OnWake() override;
	void OnDestroy() override {};
	void OnDraw() override {};
};

