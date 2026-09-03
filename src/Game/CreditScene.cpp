#include "pch.h"
#include "CreditScene.h"
#include "2DPendentLib/Debug.h"

void CreditScene::OnInitialize()
{
	CreateText("text", Vector2f{ (float)GetWindowWidth() / 2, (float)GetWindowHeight() + 400 }, 
		
		"Credits :\n\n\nDevelopment :\n\nGameplay Developer :\n\nCARMICHAEL Alexis\nMERMIN Kylian\nRANDON Gaël\nDE ROS Vivien\nSANCHEZ Maxence\nDESPESSE Nathan\n\nEngine Developer :\n\nBERNY Alexis\nRANDON Gaël\nDE ROS Vivien\nMERMIN Kylian\n\nSound Designer :\n\nCANTOREGGI Mattia\n\nGame Designer :\n\nMERMIN Kylian\n\n\nArt :\n\nBackground Designer :\n\nDESPESSE Nathan\nDE ROS Vivien\n\nProps/Items Designer :\n\nDESPESSE Nathan\n\nCharacter Designer/Animator :\n\nDESPESSE Nathan\n\n\nBusiness :\n\nManagement :\n\nLE CLECH Damien\nCORLOSQUET - LEFRANC Volodia\n\n\nSpecial Thanks :\n\nVYSTAVEL Peter\nSECCIA Sylvain\nSIMON Lennie\nLUCAS Simonn",
		ColorsNamed::LightGray ,false, false);

	m_text = GetGeometry<Text>("text");

	Entity* background = CreateEntity<Entity>(GetWindowCenter(), GetWindowSize(), "BlackBackground", Shape::Rectangle, true);
	background->GetSprite()->ResizeStretched(GetWindowSize());
	background->SyncSprites();
	background->SetDrawLayer(5004);
}

void CreditScene::OnUpdate()
{
	m_text->Move({ 0, -0.1 });
}

void CreditScene::OnWake()
{
	m_text->SetPosition(Vector2f{ (float)GetWindowWidth() / 2, (float)GetWindowHeight() + 400 });
}
