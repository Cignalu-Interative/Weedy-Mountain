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
	SetDrawLayer(INT_MAX - 20);
	GetSprite()->ResizeStretched(GetDimension());
	SyncSprites();

	m_title = Debug::Get()->CreateText("ATMtitle", GetPosition({ 0.5f, 0 }) + Vector2f(0, 75), "ATM", ColorsNamed::Black, true);
	m_title->SetFontStyle(true);
	m_title->SetFontSize(35);

	m_atmAmount = Debug::Get()->CreateText("amountText", GetPosition({ 0, 0.35f }) + Vector2f(85, 0), "Amount : ", ColorsNamed::Black, true);
	m_atmAmount->SetFontStyle(true);
	m_atmAmount->SetFontSize(32);

	m_dailyDepositError = Debug::Get()->CreateText("dailyDepositMax", GetPosition({ 0, 0.35f }) + Vector2f(500, -75), "You've reached your daily deposit amount you cannot deposit more today.", ColorsNamed::Red, true);
	m_dailyDepositError->SetFontStyle(true);
	m_dailyDepositError->SetFontSize(28);
	m_dailyDepositError->SetDrawLayer(INT_MAX);
	m_dailyDepositError->SetIsVisible(false);

	m_errorMsg = Debug::Get()->CreateText("toomuch", GetPosition({ 0, 0.35f }) + Vector2f(650, 0), "Amount too high you can only retrieve/deposit 200", ColorsNamed::Red, true);
	m_errorMsg->SetFontStyle(true);
	m_errorMsg->SetFontSize(28);
	m_errorMsg->SetDrawLayer(INT_MAX);
	m_errorMsg->SetIsVisible(false);

	m_amountBox = scene->CreateTextBox("atmTextBox", GetPosition({ 0, 0.35f }) + Vector2f(250, 0), { 150, 75 }, ColorsNamed::Black, true);
	m_amountBox->GetCollider()->SetIsTrigger(true);
	m_amountBox->SetDrawLayer(INT_MAX);
	m_amountBox->GetText()->SetColor(ColorsNamed::Black);

	m_depositPos = GetPosition({ 1,1 }) - Vector2f{ 200, 150 };
	m_retrievePos = GetPosition({ 1,1 }) - Vector2f{ 200, 250 };
	Vector2f textBoxPos = GetPosition({ 0,0 }) + Vector2f{ 200, 150 };

	m_depositButton = scene->CreateButton(m_depositPos, Vector2i(250, 100), "Bouton_ATM_Bank", "deposit", "Deposit", true);
	m_depositButton->GetCollider()->SetIsTrigger(true);
	m_depositButton->SetDrawLayer(INT_MAX);
	/*m_depositButton->SetOnHoverEnter([this] {
		m_depositButton->GetSprite()->ChangeTexture("buttonHover");
		});
	m_depositButton->SetOnHoverExit([this] {
		m_depositButton->GetSprite()->ChangeTexture("button");
		});*/
	m_depositButton->SetOnLeftClick([this] {
		StringToInt();
		Deposit(m_amount);
		});

	m_retrieveButton = scene->CreateButton(m_retrievePos, Vector2i(250, 100), "Bouton_ATM_Cash", "retrieve", "Retrieve", true);
	m_retrieveButton->GetCollider()->SetIsTrigger(true);
	m_retrieveButton->SetDrawLayer(INT_MAX);
	/*m_retrieveButton->SetOnHoverEnter([this] {
		m_retrieveButton->GetSprite()->ChangeTexture("buttonHover");
		});
	m_retrieveButton->SetOnHoverExit([this] {
		m_retrieveButton->GetSprite()->ChangeTexture("button");
		});*/
	m_retrieveButton->SetOnLeftClick([this] {
		StringToInt();
		Retrieve(m_amount);
		});

	m_10Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(95, 100), Vector2i(120, 50), "Bouton_ATM", "+10", "10", true);
	m_10Button->GetCollider()->SetIsTrigger(true);
	m_10Button->SetDrawLayer(INT_MAX);
	/*m_10Button->SetOnHoverEnter([this] {
		m_10Button->GetSprite()->ChangeTexture("buttonHover");
		});
	m_10Button->SetOnHoverExit([this] {
		m_10Button->GetSprite()->ChangeTexture("button");
		});*/
	m_10Button->SetOnLeftClick([this] {
		m_amount = 10;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});

	m_50Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(220, 100), Vector2i(120, 50), "Bouton_ATM", "+50", "50", true);
	m_50Button->GetCollider()->SetIsTrigger(true);
	m_50Button->SetDrawLayer(INT_MAX);
	/*m_50Button->SetOnHoverEnter([this] {
		m_50Button->GetSprite()->ChangeTexture("buttonHover");
		});
	m_50Button->SetOnHoverExit([this] {
		m_50Button->GetSprite()->ChangeTexture("button");
		});*/
	m_50Button->SetOnLeftClick([this] {
		m_amount = 50;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});

	m_100Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(345, 100), Vector2i(120, 50), "Bouton_ATM", "+100", "100", true);
	m_100Button->GetCollider()->SetIsTrigger(true);
	m_100Button->SetDrawLayer(INT_MAX);
	/*m_100Button->SetOnHoverEnter([this] {
		m_100Button->GetSprite()->ChangeTexture("buttonHover");
		});
	m_100Button->SetOnHoverExit([this] {
		m_100Button->GetSprite()->ChangeTexture("button");
		});*/
	m_100Button->SetOnLeftClick([this] {
		m_amount = 100;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});

	m_200Button = scene->CreateButton(GetPosition({ 0, 0.35f }) + Vector2f(470, 100), Vector2i(120, 50), "Bouton_ATM", "+200", "200", true);
	m_200Button->GetCollider()->SetIsTrigger(true);
	m_200Button->SetDrawLayer(INT_MAX);
	/*m_200Button->SetOnHoverEnter([this] {
		m_200Button->GetSprite()->ChangeTexture("buttonHover");
		});*/
	/*m_200Button->SetOnHoverExit([this] {
		m_200Button->GetSprite()->ChangeTexture("button");
		});*/
	m_200Button->SetOnLeftClick([this] {
		m_amount = 200;
		m_amountBox->SetTextInBox(std::to_string(m_amount));
		});

	m_cashAmount = Debug::Get()->CreateText("cashAmount", textBoxPos + Vector2f{ 50 , 250 }, "Cash :    " + std::to_string(GameData::Get().GetPlayerData().m_cash), ColorsNamed::Black, true);
	m_cashAmount->SetFontSize(32);
	m_cashAmount->SetFontStyle(false,true);

	m_bankAmount = Debug::Get()->CreateText("bankAmount", textBoxPos + Vector2f{ 70 , 320 }, "In Bank :    " + std::to_string(GameData::Get().GetPlayerData().m_money), ColorsNamed::Black, true);
	m_bankAmount->SetFontSize(32);
	m_bankAmount->SetFontStyle(false, true);

	Close();
}

void UiATM::Open()
{
	SceneManager::Get()->GetScene("HUD")->SetSleepDraw(true);
	Debug::Get()->GetText("UiCashText")->SetIsVisible(false);
	Debug::Get()->GetText("Date")->SetIsVisible(false);
	Debug::Get()->GetText("time")->SetIsVisible(false);
	Debug::Get()->GetText("Money")->SetIsVisible(false);
	SetIsVisible(true);

	m_100Button->SetIsVisible(true);
	m_10Button->SetIsVisible(true);
	m_200Button->SetIsVisible(true);
	m_50Button->SetIsVisible(true);

	m_depositButton->SetIsVisible(true);
	m_retrieveButton->SetIsVisible(true);

	m_title->SetIsVisible(true);
	m_atmAmount->SetIsVisible(true);
	m_cashAmount->SetIsVisible(true);
	m_bankAmount->SetIsVisible(true);

	m_amountBox->SetIsVisible(true);

	m_isOpen = true; 
}

void UiATM::Close()
{
	SceneManager::Get()->GetScene("HUD")->SetSleepDraw(false);
	Debug::Get()->GetText("UiCashText")->SetIsVisible(true);
	Debug::Get()->GetText("Date")->SetIsVisible(true);
	Debug::Get()->GetText("time")->SetIsVisible(true);
	Debug::Get()->GetText("Money")->SetIsVisible(true);
	SetIsVisible(false);

	m_100Button->SetIsVisible(false);
	m_10Button->SetIsVisible(false);
	m_200Button->SetIsVisible(false);
	m_50Button->SetIsVisible(false);

	m_depositButton->SetIsVisible(false);
	m_retrieveButton->SetIsVisible(false);

	m_errorMsg->SetIsVisible(false);
	m_dailyDepositError->SetIsVisible(false);

	m_title->SetIsVisible(false);
	m_atmAmount->SetIsVisible(false);
	m_cashAmount->SetIsVisible(false);
	m_bankAmount->SetIsVisible(false);

	m_amountBox->SetIsVisible(false);

	m_isOpen = false;
}

void UiATM::OnUpdate()
{
	m_cashAmount->SetText("Cash :    " + std::to_string(GameData::Get().GetPlayerData().m_cash));
	m_bankAmount->SetText("In Bank :    " + std::to_string(GameData::Get().GetPlayerData().m_money));

}

void UiATM::OnEvent()
{
	if(m_isVisible == false)
		return;

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

	GameData::Get().GetPlayerData().m_money -= p_amount;
	GameData::Get().GetPlayerData().m_cash += p_amount;
}

void UiATM::Deposit(int p_amount)
{
	if (GameData::Get().GetPlayerData().m_cash < p_amount)
		return;

	int result = GameData::Get().GetPlayerData().m_todayDepositCount + p_amount;

	if (result > GameData::Get().GetPlayerData().m_maxDepositPerDay)
	{
		m_dailyDepositError->SetIsVisible(true);
		return;
	}

	

	GameData::Get().GetPlayerData().m_money += p_amount;
	GameData::Get().GetPlayerData().m_cash -= p_amount;
	GameData::Get().GetPlayerData().m_todayDepositCount += p_amount;
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
			m_amount = 0;
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
