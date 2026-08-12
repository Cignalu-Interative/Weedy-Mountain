#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Animation.h"

class LoadMenu
{
private:

	Entity* m_background = nullptr;
	Entity* m_loadingAnimation = nullptr;

public:

	LoadMenu() = default;
	~LoadMenu() = default;

	void Initialize();


};

