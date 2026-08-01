#include "pch.h"
#include "GlobalMenuScene.h"

void GlobalMenuScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");

	Entity* background = CreateEntity<Entity>(GetWindowCenter(), GetWindowSize(), "InventoryWeedyMountain", Shape::Rectangle, true);
	background->GetSprite()->ResizeStretched(GetWindowSize());
	background->SyncSprites();

	Debug::Get()->CreateText("GameTitle", { background->GetPosition({0.25f, 0.02f })}, "WEEDY MOUNTAIN ", ColorsNamed::Black, true);
	Debug::Get()->GetText("GameTitle")->SetFontSize(200);

	Vector2f windowCenter = GetWindowCenter();

	Vector2f settingsButtonPos = windowCenter + Vector2f{0,100};
	Vector2f quitButtonPos = windowCenter + Vector2f{0,200};
	Vector2i buttonDim = { 200,75 };
	if (m_hasSave)
	{
		//m_newGameButton->SetIsVisible(false);
		m_continueButton = CreateButton({ windowCenter - Vector2f{0, 100} }, buttonDim, "button", "continueButton", "CONTINUE");
		m_continueButton->GetSprite()->ResizeStretched(m_continueButton->GetDimension());
		m_continueButton->SyncSprites();
		m_continueButton->SetOnHoverStay([this] {m_continueButton->SetSprite("button2"); });
		m_continueButton->SetOnHoverExit([this] {m_continueButton->SetSprite("button"); });
		Debug::Get()->GetText(m_continueButton->GetTextID())->SetFontSize(50);
		Debug::Get()->GetText(m_continueButton->GetTextID())->SetPosition(m_continueButton->GetPosition());

		m_newGameButton = CreateButton({ windowCenter }, buttonDim, "button", "newGameButton", "NEW GAME");
		m_newGameButton->GetSprite()->ResizeStretched(m_newGameButton->GetDimension());
		m_newGameButton->SyncSprites();
		m_newGameButton->SetOnHoverStay([this] {m_newGameButton->SetSprite("button2"); });
		m_newGameButton->SetOnHoverExit([this] {m_newGameButton->SetSprite("button"); });
		Debug::Get()->GetText(m_newGameButton->GetTextID())->SetFontSize(50);
		Debug::Get()->GetText(m_newGameButton->GetTextID())->SetPosition(m_newGameButton->GetPosition());
	}
	else
	{
		settingsButtonPos = windowCenter;
		quitButtonPos = windowCenter + Vector2f{ 0,100 };
		//m_newGameButton->SetIsVisible(false);

		m_startButton = CreateButton({ windowCenter - Vector2f{0, 100} }, buttonDim, "button", "startButton", "START");
		m_startButton->GetSprite()->ResizeStretched(m_startButton->GetDimension());
		m_startButton->SyncSprites();
		m_startButton->SetOnHoverStay([this] {m_startButton->SetSprite("button2"); });
		m_startButton->SetOnHoverExit([this] {m_startButton->SetSprite("button"); });
		m_startButton->SetOnLeftClick([this] {SceneManager::Get()->PushScene("HUD"); SceneManager::Get()->PushScene("game"); });
		Debug::Get()->GetText(m_startButton->GetTextID())->SetFontSize(50);
		Debug::Get()->GetText(m_startButton->GetTextID())->SetPosition(m_startButton->GetPosition());
	}



	m_settingsButton = CreateButton(settingsButtonPos, buttonDim, "button", "settingsButton", "SETTINGS");
	m_settingsButton->GetSprite()->ResizeStretched(m_settingsButton->GetDimension());
	m_settingsButton->SyncSprites();
	m_settingsButton->SetOnHoverStay([this] {m_settingsButton->SetSprite("button2"); });
	m_settingsButton->SetOnHoverExit([this] {m_settingsButton->SetSprite("button"); });
	Debug::Get()->GetText(m_settingsButton->GetTextID())->SetFontSize(50);
	Debug::Get()->GetText(m_settingsButton->GetTextID())->SetPosition(m_settingsButton->GetPosition());

	m_quitButton = CreateButton(quitButtonPos, buttonDim, "button", "quitButton", "QUIT");
	m_quitButton->GetSprite()->ResizeStretched(m_quitButton->GetDimension());
	m_quitButton->SyncSprites();
	m_quitButton->SetOnLeftClick([this] {Application::Get()->SetisRunning(false); });
	m_quitButton->SetOnHoverStay([this] {m_quitButton->SetSprite("button2"); });
	m_quitButton->SetOnHoverExit([this] {m_quitButton->SetSprite("button"); });
	Debug::Get()->GetText(m_quitButton->GetTextID())->SetFontSize(50);
	Debug::Get()->GetText(m_quitButton->GetTextID())->SetPosition(m_quitButton->GetPosition());

}

void GlobalMenuScene::OnEvent()
{
}

void GlobalMenuScene::OnUpdate()
{
}

void GlobalMenuScene::OnDestroy()
{
}

void GlobalMenuScene::OnSleep()
{
	SetSleepDraw(true);
	SetSleepEvent(true);
	SetSleepUpdate(true);

	Debug::Get()->GetText("GameTitle")->SetIsVisible(false);

	if (m_startButton)
		m_startButton->SetIsVisible(false);

	if (m_continueButton)
		m_continueButton->SetIsVisible(false);
	if(m_newGameButton)
		m_newGameButton->SetIsVisible(false);

	m_settingsButton->SetIsVisible(false);
	m_quitButton->SetIsVisible(false);

}

void GlobalMenuScene::OnWake()
{
}
