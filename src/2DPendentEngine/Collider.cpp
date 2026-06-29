#include "pch.h"
#include "Collider.h"
#include "Entity.h"
#include "Application.h"
#include "Utils.h"

bool Collider::CanCollideWith(const Collider* p_other) const
{
    return (m_mask & p_other->m_layer) != 0 && (p_other->m_mask & m_layer) != 0;
}

void Collider::SyncWithOwner()
{
    if (m_owner == nullptr)
        return;

    // Position monde = position centre de l'owner + offset relatif
    Vector2f ownerCenter;
    if(GetOwner()->GetIsGUI())
        ownerCenter = Application::Get()->GetCamera()->ScreenToWorld(m_owner->GetPosition({ 0.5f, 0.5f }));
    else
        ownerCenter = m_owner->GetPosition({ 0.5f, 0.5f });

    float cx = ownerCenter.GetX() + m_offset.GetX();
    float cy = ownerCenter.GetY() + m_offset.GetY();

    float angle = Utils::DegToRad(m_owner->GetAngle()); 

    if (angle == 0.f || m_shape == Shape::Circle)
    {
        float halfW = GetDimension().GetX() * 0.5f;
        float halfH = GetDimension().GetY() * 0.5f;
        m_cachedAABB.minX = cx - halfW;
        m_cachedAABB.maxX = cx + halfW;
        m_cachedAABB.minY = cy - halfH;
        m_cachedAABB.maxY = cy + halfH;
    }
    else
    {
        float halfW = GetDimension().GetX() * 0.5f;
        float halfH = GetDimension().GetY() * 0.5f;

        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        float corners[4][2] = 
        {
            { cx + cosA * (-halfW) - sinA * (-halfH),  cy + sinA * (-halfW) + cosA * (-halfH) }, // TL
            { cx + cosA * (halfW)-sinA * (-halfH),  cy + sinA * (halfW)+cosA * (-halfH) }, // TR
            { cx + cosA * (-halfW) - sinA * (halfH),  cy + sinA * (-halfW) + cosA * (halfH) }, // BL
            { cx + cosA * (halfW)-sinA * (halfH),  cy + sinA * (halfW)+cosA * (halfH) }, // BR
        };

        float minX = corners[0][0], maxX = corners[0][0];
        float minY = corners[0][1], maxY = corners[0][1];
        for (int i = 1; i < 4; ++i)
        {
            if (corners[i][0] < minX) 
                minX = corners[i][0];

            if (corners[i][0] > maxX) 
                maxX = corners[i][0];

            if (corners[i][1] < minY) 
                minY = corners[i][1];

            if (corners[i][1] > maxY) 
                maxY = corners[i][1];
        }

        m_cachedAABB = { minX, minY, maxX, maxY };
    }

    SetPosition({ cx, cy });
}