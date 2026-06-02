#include "pch.h"
#include "TestScene.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

#include "Button.h"

#include <iostream>

void TestScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	MapTiledManager::Get()->AddMaps("../../res/2DPendentEngine/Assets/Maps/Maps_Repertory.json");
	SoundManager::Get()->AddSound("../../res/2DPendentEngine/Assets/Sounds/Sounds_Repertory.json");
	FontManager::Get()->AddFontByRepertory("../../res/2DPendentEngine/Assets/Fonts/Fonts_Repertory.json");

	int w = GetWindowWidth();
	int h = GetWindowHeight();

	Button* b = CreateButton({ w/2.f, h/2.f}, {300, 100}, "Oak_Tree", "TEST", "START");
	b->ShowCollider(true);
	b->SetOnClick([] {
		std::cout << "click" << std::endl;
		});
	b->SetOnReleaseClick([] {
		std::cout << "release click" << std::endl;
		});
}

void TestScene::OnEvent()
{
}

void TestScene::OnUpdate()
{
}

void TestScene::OnDestroy()
{
}
