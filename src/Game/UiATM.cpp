#include "pch.h"
#include "UiATM.h"
#include "TextBox.h"
#include "2DPendentLib/InputManager.h"
#include "GameData.h"
#include <string>


void UiATM::OnInitialize()
{
	Scene* scene = GetScene();
	RemoveCollider("default");
	GetSprite()->ResizeStretched(GetDimension());
	SyncSprites();
	SetDrawLayer(INT_MAX - 4);

	m_depositPos = GetPosition({ 1,1 }) - Vector2f{ 200, 150 };
	m_retrievePos = GetPosition({ 1,1 }) - Vector2f{ 200, 250 };
	Vector2f textBoxPos = GetPosition({ 0,0 }) + Vector2f{200, 150};

	m_depositButton = scene->CreateButton(m_depositPos, Vector2i(240, 100), "button", "deposit", "Deposit", true);
	m_depositButton->SetDrawLayer(INT_MAX);
	m_depositButton->SetOnHoverEnter([this] {
		m_depositButton->GetSprite()->ChangeTexture("buttonHover");
		});
	m_depositButton->SetOnHoverExit([this] {
		m_depositButton->GetSprite()->ChangeTexture("button");
		});
	m_depositButton->SetOnLeftClick([this] {
		GameData::Get().GetPlayerData().m_money += m_amount;
		GameData::Get().GetPlayerData().m_cash -= m_amount;
		});

	m_retrieveButton = scene->CreateButton(m_retrievePos, Vector2i(240, 100), "button", "retrieve", "Retrieve", true);
	m_retrieveButton->SetDrawLayer(INT_MAX);
	m_retrieveButton->SetOnHoverEnter([this] {
		m_retrieveButton->GetSprite()->ChangeTexture("buttonHover");
		});
	m_retrieveButton->SetOnHoverExit([this] {
		m_retrieveButton->GetSprite()->ChangeTexture("button");
		});
	m_retrieveButton->SetOnLeftClick([this] {
		GameData::Get().GetPlayerData().m_money -= m_amount;
		GameData::Get().GetPlayerData().m_cash += m_amount;
		});

	m_amountBox = scene->CreateTextBox("amountBox", {200, 200}, {150, 100}, ColorsNamed::Red, true);
	m_amountBox->GetCollider()->SetIsTrigger(true);
	m_amountBox->SetDrawLayer(INT_MAX);

	m_cashAmount = Debug::Get()->CreateText("cashAmount", textBoxPos + Vector2f(0, 50), std::to_string(GameData::Get().GetPlayerData().m_cash), ColorsNamed::White, true);
	m_cashAmount->SetDrawLayer(INT_MAX);
	m_bankAmount = Debug::Get()->CreateText("bankAmount", textBoxPos + Vector2f(0, 75), std::to_string(GameData::Get().GetPlayerData().m_money), ColorsNamed::White, true);
	m_bankAmount->SetDrawLayer(INT_MAX);

}

void UiATM::OnUpdate()
{
}

void UiATM::OnEvent()
{
	InputManager* im = InputManager::Get();

	if (im->IsMouseDown(BUTTON_LEFT) && m_amountBox->IsMouseIn())
	{
		im->StartTextInput();
	}
	if (im->IsMouseDown(BUTTON_LEFT) && !m_amountBox->IsMouseIn())
	{
		im->StopTextInput();
	}
	if (im->IsTextInputActive() == true && im->IsKeyboardDown(K_DELETE))
	{
		im->DeleteLastCharInFinalTextInput();
	}
	if (im->IsTextInputActive() == true)
	{
		m_amountBox->SetTextInBox(im->GetfinalTextInput());
	}
}

