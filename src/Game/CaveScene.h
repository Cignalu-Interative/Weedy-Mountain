#pragma once
#include "2DPendentEngine/Scene.h"
#include "Player.h"
class CaveScene : public Scene
{
	Player* m_player = nullptr;

protected:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override;

	void OnWake() override;

	void OnDraw() override;

};

