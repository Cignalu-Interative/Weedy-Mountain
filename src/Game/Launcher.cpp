#include "pch.h"
#include "Launcher.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "TestScene.h"
#include "GameScene.h"
#include "VillageScene.h"

namespace Launcher
{
	void Launch()
	{
		SceneManager::Get()->AddScene<GameScene>("game");
		SceneManager::Get()->AddScene<VillageScene>("village");
		//SceneManager::Get()->AddScene<DemoScene>("demo");
		SceneManager::Get()->AddScene<TestScene>("test");
	}
}