#include "pch.h"
#include "Collider.h"
#include "Entity.h"
#include "Application.h"

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

	m_cachedAABB.minX = ownerCenter.GetX() + m_offset.GetX() - GetDimension().GetX() / 2.f;
	m_cachedAABB.maxX = ownerCenter.GetX() + m_offset.GetX() + GetDimension().GetX() / 2.f;
	m_cachedAABB.minY = ownerCenter.GetY() + m_offset.GetY() - GetDimension().GetY() / 2.f;
	m_cachedAABB.maxY = ownerCenter.GetY() + m_offset.GetY() + GetDimension().GetY() / 2.f;

    SetPosition({ ownerCenter.GetX() + m_offset.GetX(), ownerCenter.GetY() + m_offset.GetY() });
}