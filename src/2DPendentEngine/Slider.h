#pragma once
#include "Button.h"

class Slider : public Button
{
private:
    int m_stateCount;      
    bool m_isVertical;      

    int m_railWidth;
    int m_railHeight;

    int m_handleWidth;
    int m_handleHeight; 

    float m_normalizedValue = 0.f; 
    int m_currentState = 0;   

    bool  m_isDragging = false;
    float m_dragOffset = 0.f; 

    Vector2f m_railOrigin;

    std::function<void(float)> m_onValueChanged;  
    std::function<void(int)> m_onStateChanged;   

    float GetRailStart() const;   
    float GetRailLength() const;   
    void ApplyHandlePosition();  
    void SnapToNearest();   

    std::string m_idRail;
    std::string m_idHandle;
    std::string m_idStatePrefix;

public:
    Slider() = default;
    ~Slider() = default;

    void InitSlider(const std::string& p_id, int p_railWidth, int p_railHeight, int p_stateCount = -1);

    void SetOnValueChanged(std::function<void(float)> p_fn) { m_onValueChanged = p_fn; }
    void SetOnStateChanged(std::function<void(int)> p_fn) { m_onStateChanged = p_fn; }

    float GetNormalizedValue() const { return m_normalizedValue; }
    int GetCurrentState() const { return m_currentState; }

    void SetNormalizedValue(float p_value);   

    void OnUpdate() override;
};