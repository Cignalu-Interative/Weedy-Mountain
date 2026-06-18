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
#include "Slider.h"

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
	b->SetOnLeftClick([] {
		std::cout << "click" << std::endl;
		});
	b->SetOnReleaseLeftClick([] {
		std::cout << "release click" << std::endl;
		});

	Slider* s = CreateSlider({ 200.f, h / 2.f }, { 20, 500 }, "", "SLIDER");
	s->SetOnHoverStay([] {
		std::cout << "hover" << std::endl;
		});
	s->SetOnHoverExit([] {
		std::cout << "exit" << std::endl;
		});
}

void TestScene::OnEvent()
{
	if (InputManager::Get()->IsKeyboardDown(K_TAB))
	{
		//SceneManager::Get()->ClearScenesInStack();
		SceneManager::Get()->PushScene("Game");
	}
}

void TestScene::OnUpdate()
{
}

void TestScene::OnDestroy()
{
}

void TestScene::OnSleep()
{
	SetIsVisible(false);
	SetSleepEvent(true);
	SetSleepUpdate(true);
}

void TestScene::OnWake()
{
	SetIsVisible(true);
	SetSleepEvent(false);
	SetSleepUpdate(false);
}
