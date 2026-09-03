#include "pch.h"
#include "Launcher.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "TestScene.h"
#include "GameScene.h"
#include "VillageScene.h"
#include "HUDScene.h"
#include "GlobalMenuScene.h"
#include "CaveScene.h"
#include "SaveManager.h"
#include "CreditScene.h"

namespace Launcher
{
	void Launch()
	{
		SceneManager::Get()->AddScene<GlobalMenuScene>("Global");
		SceneManager::Get()->PushScene("Global");
		SceneManager::Get()->AddScene<HUDScene>("HUD");
		SceneManager::Get()->AddScene<GameScene>("game");
		//SceneManager::Get()->PushScene("game");
		SceneManager::Get()->AddScene<VillageScene>("village");
		SceneManager::Get()->AddScene<CaveScene>("cave");
		SceneManager::Get()->AddScene<CreditScene>("credit");
		//SceneManager::Get()->AddScene<DemoScene>("demo");
		//SceneManager::Get()->AddScene<TestScene>("test");
	}
}