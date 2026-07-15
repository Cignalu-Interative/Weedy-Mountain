#pragma once
#include "Entity.h".
#include <functional>

class Button : public Entity
{
private:
	std::string m_textID;

	bool m_mouseWasIn = false;

	std::function<void()> m_onLeftClick;
	std::function<void()> m_onHeldLeftClick;
	std::function<void()> m_onReleaseLeftClick;

	std::function<void()> m_onRightClick;
	std::function<void()> m_onHeldRightClick;
	std::function<void()> m_onReleaseRightClick;

	std::function<void()> m_onHoverEnter;
	std::function<void()> m_onHoverStay;
	std::function<void()> m_onHoverExit;

public:
	Button() = default;
	~Button();

	void SetOnLeftClick(std::function<void()> p_function) { m_onLeftClick = p_function; }
	void SetOnHeldLeftClick(std::function<void()> p_function) { m_onHeldLeftClick = p_function; }
	void SetOnReleaseLeftClick(std::function<void()> p_function) { m_onReleaseLeftClick = p_function; }

	void SetOnRightClick(std::function<void()> p_function) { m_onRightClick = p_function; }
	void SetOnHeldRightClick(std::function<void()> p_function) { m_onHeldRightClick = p_function; }
	void SetOnReleaseRightClick(std::function<void()> p_function) { m_onReleaseRightClick = p_function; }

	void SetOnHoverEnter(std::function<void()> p_function) { m_onHoverEnter = p_function; }
	void SetOnHoverStay(std::function<void()> p_function) { m_onHoverStay = p_function; }
	void SetOnHoverExit(std::function<void()> p_function) { m_onHoverExit = p_function; }

	void InitText(const std::string& p_textID, const std::string& p_text);
	void SetText(const std::string& p_text);
	void SetTextOffset(const Vector2f& p_offset);

	bool IsMouseIn();

	void OnUpdate() override;
	std::string GetTextID() { return m_textID; }

	void SetIsVisible(bool p_isVisible);
};

