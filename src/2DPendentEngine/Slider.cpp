#include "pch.h"
#include "Slider.h"
#include "InputManager.h"
#include "Debug.h"

#include <iostream>

void Slider::InitSlider(const std::string& p_id, int p_railWidth, int p_railHeight, int p_stateCount)
{
    m_railWidth = p_railWidth;
    m_railHeight = p_railHeight;
    m_stateCount = p_stateCount;
    m_isVertical = (p_railHeight > p_railWidth);
    m_railOrigin = GetPosition({ 0.f, 0.f });

    m_idRail = p_id + "_rail";
    m_idHandle = p_id + "_handle";
    m_idStatePrefix = p_id + "_state_";

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

    GetCollider()->SetDimension({ m_handleWidth, m_handleHeight });
    Transformable::SetDimension({ m_handleWidth, m_handleHeight });
    GetSprite()->Resize({ m_handleWidth, m_handleHeight });

    bool isGUI = GetIsGUI();

    Debug::Get()->CreateFillRect(m_idRail,{ m_railOrigin.GetX() + m_railWidth / 2.f, m_railOrigin.GetY() + m_railHeight / 2.f }, { m_railWidth, m_railHeight }, Color{ 255, 255, 255, 255 }, isGUI);

    if (m_stateCount >= 2)
    {
        float step = 1.f / static_cast<float>(m_stateCount - 1);

        int barW = m_isVertical ? m_railWidth : 2;
        int barH = m_isVertical ? 2 : m_railHeight;

        for (int i = 0; i < m_stateCount; i++)
        {
            float normalized = i * step;
            float posOnAxis = GetRailStart() + normalized * GetRailLength();

            Vector2f barPos;

            if (m_isVertical)
                barPos = { m_railOrigin.GetX() + m_railWidth / 2.f, posOnAxis };
            else
                barPos = { posOnAxis, m_railOrigin.GetY() + m_railHeight / 2.f };

            Debug::Get()->CreateFillRect(m_idStatePrefix + std::to_string(i), barPos, { barW, barH }, Color{ 150, 150, 150, 255 }, isGUI);
        }
    }

    Debug::Get()->CreateFillRect(m_idHandle, { m_railOrigin.GetX() + m_railWidth / 2.f, m_railOrigin.GetY() + m_railHeight / 2.f }, { m_handleWidth, m_handleHeight }, Color{ 245, 222, 179, 255 }, isGUI);

    ApplyHandlePosition();
}

float Slider::GetRailStart() const
{
    if (m_isVertical)
        return m_railOrigin.GetY() + m_handleHeight / 2.f;
    else
        return m_railOrigin.GetX() + m_handleWidth / 2.f;
}

void Slider::ApplyHandlePosition()
{
    float offset = m_normalizedValue * GetRailLength();

    float x, y;

    if (m_isVertical)
    {
        x = m_railOrigin.GetX() + m_railWidth / 2.f;
        y = GetRailStart() + offset;
    }
    else
    {
        x = GetRailStart() + offset;
        y = m_railOrigin.GetY() + m_railHeight / 2.f;
    }

    SetPosition({ x, y });

    if (m_idHandle.empty() == false)
    {
        Debug::Get()->GetFillRect(m_idHandle)->SetPosition({ x, y });
    }
}

float Slider::GetRailLength() const
{
    if (m_isVertical)
        return static_cast<float>(m_railHeight - m_handleHeight);
    else
        return static_cast<float>(m_railWidth  - m_handleWidth);
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

    if (m_isDragging && im->IsMouseUp(BUTTON_LEFT))
    {
        m_isDragging = false;

        if (m_stateCount != -1)
            SnapToNearest();

        if (m_onValueChanged)
            m_onValueChanged(m_normalizedValue);

        Button::OnUpdate();
        return; 
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

    Button::OnUpdate();
}