#pragma once
#include "Entity.h"

class UiChest;
class Chest : public Entity
{
private :

	UiChest* m_uiChest;

	bool m_isColliding = false;

	void OnInitialize() override;
	void OnEvent() override;

	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;
public : 

};

