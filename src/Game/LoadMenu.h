#pragma once
#include "2DPendentEngine/Entity.h"
#include "2DPendentEngine/Animation.h"
#include <string>

class Scene;
class LoadMenu
{
private:

	Entity* m_background = nullptr;
	Entity* m_loadingAnimation = nullptr;

	std::string m_name = "";

public:

	LoadMenu() = default;
	~LoadMenu() = default;

	void InitializeLoadMenu(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, bool p_animation, const std::string& p_jsonAnimation, Scene* p_scene, bool p_isGUI = true);

	void SetName(const std::string& p_name) { m_name = p_name; }

	const std::string& GetName() const { return m_name; }
};

