#include "pch.h"
#include "Launcher.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "TestScene.h"
#include "GameScene.h"
#include "VillageScene.h"
#include "HUDScene.h"
#include "CaveScene.h"

namespace Launcher
{
	void Launch()
	{
		SceneManager::Get()->AddScene<HUDScene>("HUD");
		SceneManager::Get()->AddScene<GameScene>("game");
		SceneManager::Get()->PushScene("game");
		SceneManager::Get()->AddScene<VillageScene>("village");
		SceneManager::Get()->AddScene<CaveScene>("cave");
		//SceneManager::Get()->AddScene<DemoScene>("demo");
		//SceneManager::Get()->AddScene<TestScene>("test");
	}
}