#include "pch.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"

inline InputManager& input = *InputManager::Get();

void User::OnInitialize()
{

}

void User::OnEvent()
{
	/*if (input.IsKeyboardHeld(K_UP))
	{
		SetDirection({ GetDirection().GetX(), -1.0f });
	}
	if (input.IsKeyboardHeld(K_DOWN))
	{
		SetDirection({ GetDirection().GetX(), 1.0f });
	}
	if (input.IsKeyboardHeld(K_LEFT))
	{
		SetDirection({ -1.0f, GetDirection().GetY() });
	}
	if (input.IsKeyboardHeld(K_RIGHT))
	{
		SetDirection({ 1.0f, GetDirection().GetY() });
	}*/
}

void User::OnUpdate()
{

}

void User::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void User::OnDestroy()
{

}