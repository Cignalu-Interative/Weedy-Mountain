#pragma once
#include "Entity.h"

class CaveTile : public Entity
{
	bool m_canGrow = true;
public:
	const bool CanGrow() { return m_canGrow; }
	void SetCanGrow(bool p_canGrow) { m_canGrow = p_canGrow; }
};

