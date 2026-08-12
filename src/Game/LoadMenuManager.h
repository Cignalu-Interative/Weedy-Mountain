#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "LoadMenu.h"

class LoadMenuManager
{
private:

	std::unordered_map<std::string, size_t> m_loadMenuData;
	std::vector<LoadMenu*> m_loadMenuVector;


public:

	static LoadMenuManager* Get();

	LoadMenuManager() = default;
	~LoadMenuManager() = default;



};

