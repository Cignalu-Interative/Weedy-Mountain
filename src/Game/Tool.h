#pragma once
#include "2DPendentEngine/Entity.h"

class Tool : public Entity
{
private:


public:
	void OnUpdate() override;
	void OnInitialize() override;

	virtual void Use();

};

