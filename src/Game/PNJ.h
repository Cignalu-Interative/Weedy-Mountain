#pragma once
#include "2DPendentEngine/Entity.h"

class PNJ : public Entity
{
private : 
	bool m_isInPlayerZone = false;
	bool m_isTalking = false;

public : 

	void OnInitialize() override;
	void OnEvent() override;


	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;

	void Talk(const std::string& p_say); // parametre a changer pour mettre la target du dialogue a recuperer
	void EndTalk();
	bool IsTalking() { return m_isTalking; }
};

