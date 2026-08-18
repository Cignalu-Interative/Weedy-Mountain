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
class NightEffect;
class HUDScene : public Scene
{
private:
	PlayerInventory* m_playerInventory = nullptr;
	UiCollection* m_collectionUi = nullptr;
	QuestMenu* m_questMenu = nullptr;

	UiJobs* m_jobsUi = nullptr;

	NightEffect* m_nightEffect = nullptr;

	Entity* m_backgroundUiTimeDate = nullptr;
	UiStats* m_statsUi = nullptr;
	UiMoney* m_moneyUi = nullptr;
	UiCash* m_cashUi = nullptr;
	UiTime* m_timeUi = nullptr;
	UiDate* m_dateUi = nullptr;

	RecipesUi* m_recipe = nullptr;
	UiShop* m_shop = nullptr;

	Vector2i m_lastWindowSize = {};

	Button* m_jobsButton = nullptr;
	Button* m_questButton = nullptr;
	Button* m_recipiesButton = nullptr;
	Button* m_collectionButton = nullptr;
	Entity* m_barButtonEntity = nullptr;

protected:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override {};

	void OnWake() override {};

	void OnDraw() override {};

	void UiPos();

	void BarButtonInit();

	void CloseAllUi();


public : 

	void HideUi();

	void ShowUi();

	void OpenUi(Entity* p_Ui);

};

