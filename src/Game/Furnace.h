#pragma once
#include "Entity.h"

class FurnaceUi;
class Furnace : public Entity
{
private :

	FurnaceUi* m_furnaceUi = nullptr;
	
	bool m_isColliding = false;

	void OnInitialize() override;
	void OnEvent() override;
	void OnUpdate() override;

	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;
public:

};