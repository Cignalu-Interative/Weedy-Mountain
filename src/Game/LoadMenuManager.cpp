#include "pch.h"
#include "LoadMenuManager.h"

LoadMenuManager* LoadMenuManager::Get()
{
	static LoadMenuManager instance;
	return &instance;
}
