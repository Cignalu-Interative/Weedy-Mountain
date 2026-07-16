#include "pch.h"
#include "CaveScene.h"
#include "EnviroProps.h"
#include "EnviroSpawn.h"

#include "ItemOnGround.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "ItemID.h"
#include "ItemStack.h"

#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

void CaveScene::OnInitialize()
{
}

void CaveScene::OnEvent()
{
}

void CaveScene::OnUpdate()
{
}

void CaveScene::OnDestroy()
{
}

void CaveScene::OnSleep()
{
	SceneManager::Get()->GetScene("cave")->SetIsVisible(false);
	SetSleepEvent(true);
}

void CaveScene::OnWake()
{
	SceneManager::Get()->GetScene("cave")->SetIsVisible(true);
	MapTiledManager::Get()->SetCurrentMap("Cave");
}
