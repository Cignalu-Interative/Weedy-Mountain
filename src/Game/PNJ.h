#pragma once
#include "2DPendentEngine/Entity.h"

class PNJ : public Entity
{
private : 
	bool m_isInPlayerZone = false;

public : 

	void OnInitialize() override;
	void OnEvent() override;


	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;


};

