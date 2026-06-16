#pragma once
#include "Entity.h".
#include <functional>

class Button : public Entity
{
private:
	std::string m_textID;

	bool m_mouseWasIn = false;

	std::function<void()> m_onClick;
	std::function<void()> m_onHeldClick;
	std::function<void()> m_onReleaseClick;

	std::function<void()> m_onHoverEnter;
	std::function<void()> m_onHoverStay;
	std::function<void()> m_onHoverExit;

public:
	Button() = default;
	~Button();

	void SetOnClick(std::function<void()> p_function) { m_onClick = p_function; }
	void SetOnHeldClick(std::function<void()> p_function) { m_onHeldClick = p_function; }
	void SetOnReleaseClick(std::function<void()> p_function) { m_onReleaseClick = p_function; }

	void SetOnHoverEnter(std::function<void()> p_function) { m_onHoverEnter = p_function; }
	void SetOnHoverStay(std::function<void()> p_function) { m_onHoverStay = p_function; }
	void SetOnHoverExit(std::function<void()> p_function) { m_onHoverExit = p_function; }

	void InitText(const std::string& p_textID, const std::string& p_text);
	void SetText(const std::string& p_text);

	bool IsMouseIn();

	void OnUpdate() override;
};

