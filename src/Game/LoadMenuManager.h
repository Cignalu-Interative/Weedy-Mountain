#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "LoadMenu.h"

class Scene;
class LoadMenuManager
{
private:

	std::unordered_map<std::string, size_t> m_loadMenus;
	std::vector<LoadMenu*> m_loadMenuVector;

	Scene* m_scene = nullptr;

public:

	static LoadMenuManager* Get();

	LoadMenuManager() = default;
	~LoadMenuManager() = default;

	void SetScene(Scene* p_scene) { m_scene = p_scene; }

	void CreateLoadMenu(Vector2f p_position, Vector2i p_dimension, const std::string& p_id, bool p_animation, const std::string& p_jsonAnimation, bool p_isGUI = true);

	void RemoveLoadMenu(const std::string& p_id);

	void RemoveAllLoadMenus();

	LoadMenu* GetLoadMenu(const std::string& p_id) const;
};

