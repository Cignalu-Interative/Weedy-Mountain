#pragma once
#include "2DPendentEngine\Scene.h"


class Player;
class PNJ;
class UiShop;
class VillageScene : public Scene
{
private :
	Player* m_player = nullptr;
	PNJ* m_shopPNJ = nullptr;
	UiShop* m_shop = nullptr;
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

