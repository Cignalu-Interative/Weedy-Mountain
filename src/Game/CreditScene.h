#pragma once
#include "Scene.h"

class CreditScene : public Scene
{
private:

	Text* m_text;

	void OnInitialize() override;
	void OnEvent() override {};
	void OnUpdate() override;
	void OnSleep() override {};
	void OnWake() override;
	void OnDestroy() override {};
	void OnDraw() override {};
};

