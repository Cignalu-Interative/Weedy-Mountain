#pragma once
#include "Entity.h"

class Button;
class UiCollection : public Entity
{
private : 
	Button* m_next = nullptr;
	Button* m_previous = nullptr;

	Vector2f m_slotStartPos = {};
public : 

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;

	void UpdateItems();
};

