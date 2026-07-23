#pragma once
#include "2DPendentEngine\Scene.h"


class Player;
class PNJ;
class shopPNJ;
class UiShop;
class UiDealerShop;
class ATM;
class VillageScene : public Scene
{
private :
	ATM* m_atm = nullptr;
	Player* m_player = nullptr;
	shopPNJ* m_shopPNJ = nullptr;
	PNJ* m_PNJ = nullptr;
	shopPNJ* m_drugDealer = nullptr;
	UiShop* m_shop = nullptr;
	UiDealerShop* m_dealerShop = nullptr;
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

