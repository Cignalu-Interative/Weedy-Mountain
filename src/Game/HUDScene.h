#pragma once
#include "2DPendentEngine\Scene.h"

class PlayerInventory;
class UiStats;
class UiMoney;
class UiCash;
class UiTime;
class UiDate;
class RecipesUi;
class HUDScene : public Scene
{
private:
	PlayerInventory* m_playerInventory = nullptr;

	UiStats* m_statsUi = nullptr;
	UiMoney* m_moneyUi = nullptr;
	UiCash* m_cashUi = nullptr;
	UiTime* m_timeUi = nullptr;
	UiDate* m_dateUi = nullptr;

	RecipesUi* m_recipe = nullptr;
protected:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override {};

	void OnWake() override {};

	void OnDraw() override {};


public : 

	void HideUi();

	void ShowUi();
};

