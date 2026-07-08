#pragma once
#include "2DPendentEngine/Scene.h"
#include "Player.h"

enum CollisionLayers : uint32_t
{
	NoNE = 0,
	PLAYER = 1 << 0,
	ENVIRO = 1 << 1,
	FARMTILE = 1 << 2,
};

class UiStats;
class UiMoney;
class UiCash;
class UiTime;
class UiDate;

class GameScene : public Scene
{
private:
	Player* m_player = nullptr;

	Player* m_player2 = nullptr;


	UiStats* e = nullptr;
	UiMoney* f = nullptr;
	UiCash * g = nullptr;
	UiTime* h = nullptr;
	UiDate* i = nullptr;

protected:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override;

	void OnWake() override;

	void OnDraw() override;

public:

	GameScene() = default;
	~GameScene() = default;

	Player* GetPlayer() { return m_player; }

	Player* GetPlayer2() { return m_player2; }
};

