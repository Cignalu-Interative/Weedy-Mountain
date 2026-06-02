#include "pch.h"
#include "Slider.h"
#include "InputManager.h"
#include "Debug.h"

#include <iostream>

void Slider::InitSlider(int p_railWidth, int p_railHeight, int p_stateCount)
{
    m_railWidth = p_railWidth;
    m_railHeight = p_railHeight;
    m_stateCount = p_stateCount;
    m_isVertical = (p_railHeight > p_railWidth);

    Debug::Get()->CreateRect("TEST2", GetPosition(), { m_railWidth, m_railHeight }, Color{ 0, 255, 0, 255 }, true);

    if (m_isVertical)
    {
        m_handleWidth = p_railWidth;
        m_handleHeight = static_cast<int>(p_railWidth * 1.5f);
    }
    else
    {
        m_handleWidth = static_cast<int>(p_railHeight * 1.5f);
        m_handleHeight = p_railHeight;
    }

    //SetDimension({ m_handleWidth, m_handleHeight });
    ShowCollider(true);
    ApplyHandlePosition(); 

    //SetPosition(GetCollider()->GetPosition({ 0.f, 0.f }), { 0.f, 0.f });
    //std::cout << "x : " << GetPosition({ 0.f, 0.f }).GetX() << "; y : " << GetPosition({ 0.f, 0.f }).GetY() << "/ Collider pos / x : " << GetCollider()->GetPosition({ 0.f, 0.f }).GetX() << "; y : " << GetCollider()->GetPosition({ 0.f, 0.f }).GetY() << std::endl;
}

float Slider::GetRailStart() const
{
    Vector2f railOrigin = GetPosition({ 0.f, 0.f });

    if (m_isVertical)
        return railOrigin.GetY() + m_handleHeight / 2.f;
    else
        return railOrigin.GetX() + m_handleWidth / 2.f;
}

float Slider::GetRailLength() const
{
    if (m_isVertical)
        return static_cast<float>(m_railHeight - m_handleHeight);
    else
        return static_cast<float>(m_railWidth  - m_handleWidth);
}

void Slider::ApplyHandlePosition()
{
    Vector2f railOrigin = GetPosition({ 0.f, 0.f });

    float offset = m_normalizedValue * GetRailLength();
    std::cout << offset << std::endl;

    if (m_isVertical)
    {
        float x = railOrigin.GetX() + m_railWidth  / 2.f;
        float y = GetRailStart() + offset;
        SetPosition({ x, y });
    }
    else
    {
        float x = GetRailStart() + offset;
        float y = railOrigin.GetY() + m_railHeight / 2.f;
        SetPosition({ x, y });
    }
}

void Slider::SnapToNearest()
{
    if (m_stateCount < 2)
        return;  

    float step = 1.f / static_cast<float>(m_stateCount - 1);

    int nearestState = static_cast<int>(std::round(m_normalizedValue / step));

    nearestState = std::max(0, std::min(nearestState, m_stateCount - 1));

    float snappedValue = nearestState * step;

    bool stateChanged = (nearestState != m_currentState);

    m_currentState = nearestState;
    m_normalizedValue = snappedValue;

    ApplyHandlePosition();

    if (stateChanged && m_onStateChanged)
        m_onStateChanged(m_currentState);
}

void Slider::SetNormalizedValue(float p_value)
{
    m_normalizedValue = std::max(0.f, std::min(1.f, p_value));

    if (m_stateCount != -1)
        SnapToNearest();
    else
        ApplyHandlePosition();

    if (m_onValueChanged)
        m_onValueChanged(m_normalizedValue);
}

void Slider::OnUpdate()
{
    InputManager* im = InputManager::Get();

    if (m_isDragging == false && IsMouseIn() && im->IsMouseDown(BUTTON_LEFT))
    {
        m_isDragging = true;

        Vector2f mousePos = im->GetMousePosition();
        Vector2f handlePos = GetPosition({ 0.5f, 0.5f });

        m_dragOffset = m_isVertical ? mousePos.GetY() - handlePos.GetY() : mousePos.GetX() - handlePos.GetX();
    }

    if (m_isDragging && im->IsMouseHeld(BUTTON_LEFT))
    {
        Vector2f mousePos = im->GetMousePosition();

        float mousePosOnAxis = m_isVertical ? mousePos.GetY() : mousePos.GetX();
        float railStart = GetRailStart();
        float railLength = GetRailLength();

        float raw = (mousePosOnAxis - m_dragOffset - railStart) / railLength;
        float clamped = std::max(0.f, std::min(1.f, raw));

        bool changed = (clamped != m_normalizedValue);
        m_normalizedValue = clamped;

        ApplyHandlePosition();

        if (changed && m_onValueChanged)
            m_onValueChanged(m_normalizedValue);
    }

    if (m_isDragging && im->IsMouseUp(BUTTON_LEFT))
    {
        m_isDragging = false;

        if (m_stateCount != -1)
            SnapToNearest();   

        if (m_onValueChanged)
            m_onValueChanged(m_normalizedValue);
    }

    Button::OnUpdate();
}