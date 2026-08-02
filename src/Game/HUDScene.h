#pragma once
#include "2DPendentEngine\Scene.h"

class PlayerInventory;
class UiCollection;
class QuestMenu;
class UiJobs;
class UiStats;
class UiMoney;
class UiCash;
class UiTime;
class UiDate;
class RecipesUi;
class UiShop;
class DataStorage;
class HUDScene : public Scene
{
private:
	PlayerInventory* m_playerInventory = nullptr;
	UiCollection* m_collectionUi = nullptr;
	QuestMenu* m_questMenu = nullptr;

	UiJobs* m_jobsUi = nullptr;

	UiStats* m_statsUi = nullptr;
	UiMoney* m_moneyUi = nullptr;
	UiCash* m_cashUi = nullptr;
	UiTime* m_timeUi = nullptr;
	UiDate* m_dateUi = nullptr;

	RecipesUi* m_recipe = nullptr;
	UiShop* m_shop = nullptr;

	DataStorage* m_dataStorage = nullptr;

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

