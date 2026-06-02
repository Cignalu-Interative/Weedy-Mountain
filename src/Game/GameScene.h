#pragma once
#include "2DPendentEngine/Scene.h"
#include "Player.h"
class GameScene : public Scene
{
private:
	Player* m_Player = nullptr;

protected:

	void OnInitialize() override;
	
	void OnEvent() override;

	void OnUpdate() override;
	
	void OnDestroy() override;

public:


};

