#pragma once
#include "Entity.h"

class TextBox;
class UiATM : public Entity
{
private : 
	std::string m_atmAmountText = "";

	Text* m_title = nullptr; 
	Text* m_atmAmount = nullptr;

	Text* m_errorMsg = nullptr;
	Text* m_dailyDepositError = nullptr;

	Text* m_cashAmount = nullptr; 
	Text* m_bankAmount = nullptr;

	TextBox* m_amountBox = nullptr;

	Button* m_depositButton = nullptr; 
	Button* m_retrieveButton = nullptr;

	Button* m_10Button = nullptr;
	Button* m_50Button = nullptr;
	Button* m_100Button = nullptr;
	Button* m_200Button = nullptr;

	Vector2f m_depositPos = {};
	Vector2f m_retrievePos = {};
	Vector2f m_amountPos = {};

	int m_amount = 20;


	bool m_isOpen = false;

protected: 

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override; 

	void Retrieve(int p_amount);
	void Deposit(int p_amount);

	void StringToInt();
public : 

	void Open();
	void Close();

	bool IsOpen() { return m_isOpen; }

};

