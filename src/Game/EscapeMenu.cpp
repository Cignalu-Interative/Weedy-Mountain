#include "pch.h"
#include "EscapeMenu.h"

void EscapeMenu::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");

	Entity* background = CreateEntity<Entity>(GetWindowCenter(), GetWindowSize(), "InventoryWeedyMountain", Shape::Rectangle, true);
	background->GetSprite()->ResizeStretched(GetWindowSize());
	background->SyncSprites();

	Vector2f windowCenter = GetWindowCenter();
	Vector2i buttonDim = { 200,75 };

	Vector2f settingsButtonPos = windowCenter + Vector2f{ 0,50 };
	Vector2f quitButtonPos = windowCenter + Vector2f{ 0,150 };
	Vector2f saveButtonPos = windowCenter - Vector2f{ 0,50 };
	Vector2f continueButtonPos = windowCenter - Vector2f{ 0,150 };

	m_continueButton = CreateButton({ continueButtonPos }, buttonDim, "button", "continueButton", "CONTINUE");
	m_continueButton->GetSprite()->ResizeStretched(m_continueButton->GetDimension());
	m_continueButton->SyncSprites();
	m_continueButton->SetOnLeftClick([this] {SceneManager::Get()->PushScene("HUD"); SceneManager::Get()->PushScene("game"); });
	m_continueButton->SetOnHoverStay([this] {m_continueButton->SetSprite("button2"); });
	m_continueButton->SetOnHoverExit([this] {m_continueButton->SetSprite("button"); });
	Debug::Get()->GetText(m_continueButton->GetTextID())->SetFontSize(50);
	Debug::Get()->GetText(m_continueButton->GetTextID())->SetPosition(m_continueButton->GetPosition());

	m_saveButton = CreateButton({ saveButtonPos }, buttonDim, "button", "saveButton", "SAVE");
	m_saveButton->GetSprite()->ResizeStretched(m_saveButton->GetDimension());
	m_saveButton->SyncSprites();
	m_saveButton->SetOnHoverStay([this] {m_saveButton->SetSprite("button2"); });
	m_saveButton->SetOnHoverExit([this] {m_saveButton->SetSprite("button"); });
	Debug::Get()->GetText(m_saveButton->GetTextID())->SetFontSize(50);
	Debug::Get()->GetText(m_saveButton->GetTextID())->SetPosition(m_saveButton->GetPosition());

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

void EscapeMenu::OnEvent()
{
}

void EscapeMenu::OnUpdate()
{
}

void EscapeMenu::OnDestroy()
{
}

void EscapeMenu::OnSleep()
{
	SetSleepDraw(true);
	SetSleepEvent(true);
	SetSleepUpdate(true);

	m_continueButton->SetIsVisible(false);
	m_saveButton->SetIsVisible(false);
	m_settingsButton->SetIsVisible(false);
	m_quitButton->SetIsVisible(false);
}

void EscapeMenu::OnWake()
{
	SetSleepDraw(false);
	SetSleepEvent(false);
	SetSleepUpdate(false);

	m_continueButton->SetIsVisible(true);
	m_saveButton->SetIsVisible(true);
	m_settingsButton->SetIsVisible(true);
	m_quitButton->SetIsVisible(true);
}
