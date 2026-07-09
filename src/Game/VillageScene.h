#pragma once
#include "2DPendentEngine\Scene.h"


class Player;
class VillageScene : public Scene
{
private :
	Player* m_player = nullptr;

	Vector2f m_spawnPoint;

protected :

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override;

	void OnWake() override;

	void OnDraw() override;

public :

};

