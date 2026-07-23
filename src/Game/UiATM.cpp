#include "pch.h"
#include "UiATM.h"
#include "TextBox.h"
#include "2DPendentLib/InputManager.h"
#include "HUDScene.h"
#include "GameData.h"

#include "Ui/UiCash.h"
#include "Ui/UiMoney.h"

#include <string>


void UiATM::OnInitialize()
{
	Scene* scene = GetScene();

	RemoveCollider("default");
	SetDrawLayer(700);
	GetSprite()->ResizeStretched(GetDimension());
	SyncSprites();

	m_title = Debug::Get()->CreateText("title", GetPosition({ 0.5f, 0 }) + Vector2f(0, 75), "ATM", ColorsNamed::Black, true);
	m_title->SetFontStyle(true);
	m_title->SetFontSize(35);

	m_atmAmount = Debug::Get()->CreateText("amountText", GetPosition({ 0, 0.35f }) + Vector2f(85, 0), "Amount : ", ColorsNamed::Black, true);
	m_atmAmount->SetFontStyle(true);
	m_atmAmount->SetFontSize(32);

	m_errorMsg = Debug::Get()->CreateText("toomuch", GetPosition({ 0, 0.35f }) + Vector2f(750, 0), "Amount too high you can only retrieve/deposit 200", ColorsNamed::Red, true);
	m_errorMsg->SetFontStyle(true);
	m_errorMsg->SetFontSize(24);
	m_errorMsg->SetDrawLayer(INT_MAX);
	m_errorMsg->SetIsVisible(false);

	m_amountBox = scene->CreateTextBox("atmTextBox", GetPosition({ 0, 0.35f }) + Vector2f(250, 0), { 150, 75 }, ColorsNamed::Black, true);
	m_amountBox->GetCollider()->SetIsTrigger(true);
	m_amountBox->SetDrawLayer(INT_MAX);
	m_amountBox->GetText()->SetColor(ColorsNamed::Black);

	m_depositPos = GetPosition({ 1,1 }) - Vector2f{ 200, 150 };
	m_retrievePos = GetPosition({ 1,1 }) - Vector2f{ 200, 250 };
	Vector2f textBoxPos = GetPosition({ 0,0 }) + Vector2f{ 200, 150 };

	m_depositButton = scene->CreateButton(m_depositPos, Vector2i(240, 100), "button", "deposit", "Deposit", true);
	m_depositButton->SetDrawLayer(INT_MAX);
	m_depositButton->SetOnHoverEnter([this] {
		m_depositButton->GetSprite()->ChangeTexture("buttonHover");
		});
	m_depositButton->SetOnHoverExit([this] {
		m_depositButton->GetSprite()->ChangeTexture("button");
		});
	m_depositButton->SetOnLeftClick([this] {
		StringToInt();
		Deposit(m_amount);
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
		StringToInt();
		Retrieve(m_amount);
		});


	m_10Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(85, 100), Vector2i(120, 50), "button", "+10", "10", true);
	m_10Button->SetDrawLayer(INT_MAX);
	m_10Button->SetOnHoverEnter([this] {
		m_10Button->GetSprite()->ChangeTexture("buttonHover");
		});
	m_10Button->SetOnHoverExit([this] {
		m_10Button->GetSprite()->ChangeTexture("button");
		});
	m_10Button->SetOnLeftClick([this] {
		m_amount = 10;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});


	m_50Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(210, 100), Vector2i(120, 50), "button", "+50", "50", true);
	m_50Button->SetDrawLayer(INT_MAX);
	m_50Button->SetOnHoverEnter([this] {
		m_50Button->GetSprite()->ChangeTexture("buttonHover");
		});
	m_50Button->SetOnHoverExit([this] {
		m_50Button->GetSprite()->ChangeTexture("button");
		});
	m_50Button->SetOnLeftClick([this] {
		m_amount = 50;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});



	m_100Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(335, 100), Vector2i(120, 50), "button", "+100", "100", true);
	m_100Button->SetDrawLayer(INT_MAX);
	m_100Button->SetOnHoverEnter([this] {
		m_100Button->GetSprite()->ChangeTexture("buttonHover");
		});
	m_100Button->SetOnHoverExit([this] {
		m_100Button->GetSprite()->ChangeTexture("button");
		});
	m_100Button->SetOnLeftClick([this] {
		m_amount = 100;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});

	m_200Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(460, 100), Vector2i(120, 50), "button", "+200", "200", true);
	m_200Button->SetDrawLayer(INT_MAX);
	m_200Button->SetOnHoverEnter([this] {
		m_200Button->GetSprite()->ChangeTexture("buttonHover");
		});
	m_200Button->SetOnHoverExit([this] {
		m_200Button->GetSprite()->ChangeTexture("button");
		});
	m_200Button->SetOnLeftClick([this] {
		m_amount = 200;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});

	m_cashAmount = Debug::Get()->CreateText("cashAmount", textBoxPos + Vector2f{ 70 , 250 }, "Cash :    " + std::to_string(GameData::Get().GetPlayerData().m_cash), ColorsNamed::Black, true);
	m_cashAmount->SetFontSize(28);
	m_bankAmount = Debug::Get()->CreateText("bankAmount", textBoxPos + Vector2f{ 70 , 320 }, "In Bank :    " + std::to_string(GameData::Get().GetPlayerData().m_money), ColorsNamed::Black, true);
	m_bankAmount->SetFontSize(28);

}

void UiATM::Open()
{
	HUDScene* hud = GetScene<HUDScene>();
	hud->SetSleepDraw(true);

	GameData::Get().GetPlayerData().m_playerInventory->GetHotbar()->SetIsVisible(false);
}

void UiATM::Close()
{
	HUDScene* hud = GetScene<HUDScene>();
	hud->SetSleepDraw(false);

	GameData::Get().GetPlayerData().m_playerInventory->GetHotbar()->SetIsVisible(true);
}

void UiATM::OnUpdate()
{
	m_cashAmount->SetText("Cash :    " + std::to_string(GameData::Get().GetPlayerData().m_cash));
	m_bankAmount->SetText("In Bank :    " + std::to_string(GameData::Get().GetPlayerData().m_money));


	std::cout << "text box : " << m_amountBox->GetDrawLayer() << std::endl;
	std::cout << "ui : " << GetDrawLayer() << std::endl;
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
		m_atmAmountText = im->GetfinalTextInput();
		m_amountBox->SetTextInBox(m_atmAmountText);
	}
}

void UiATM::Retrieve(int p_amount)
{
	if (GameData::Get().GetPlayerData().m_money < p_amount)
		return;

	GameData::Get().GetPlayerData().m_money -= m_amount;
	GameData::Get().GetPlayerData().m_cash += m_amount;
}

void UiATM::Deposit(int p_amount)
{

	if (GameData::Get().GetPlayerData().m_cash < p_amount)
		return;

	GameData::Get().GetPlayerData().m_money += m_amount;
	GameData::Get().GetPlayerData().m_cash -= m_amount;
}

void UiATM::StringToInt()
{
	/*std::cout << "[DEBUG] longueur = " << m_atmAmountText.size() << std::endl;
	std::cout << "[DEBUG] contenu = \"" << m_atmAmountText << "\"" << std::endl;*/

	if (m_atmAmountText.empty())
		return;

	try
	{
		m_amount = std::stoi(m_atmAmountText);
		if (m_amount > 200)
		{
			m_amount = 200;
			m_amountBox->SetTextInBox(std::to_string(m_amount));
			m_errorMsg->SetIsVisible(true);
		}
		std::cout << "[UiATM AMOUNT] : " << m_amount << std::endl;
	}
	catch (const std::invalid_argument&)
	{
		m_amount = 0;
		std::cout << "[UiATM AMOUNT] : invalid arguments " << std::endl;
	}
	catch (const std::out_of_range&)
	{
		m_amount = 0;
		std::cout << "[UiATM AMOUNT] : out of range " << std::endl;
	}
}
