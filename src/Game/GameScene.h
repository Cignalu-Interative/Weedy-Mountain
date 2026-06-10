#pragma once
#include "2DPendentEngine/Scene.h"
#include "Player.h"
#include "PNJ.h"

class GameScene : public Scene
{
private:
	Player* m_Player = nullptr;
	PNJ* m_pnj = nullptr;
protected:

	void OnInitialize() override;
	
	void OnEvent() override;

	void OnUpdate() override;
	
	void OnDestroy() override;

	void OnSleep() override;

	void OnWake() override;
public:

	GameScene() = default;
	~GameScene() = default;
};

