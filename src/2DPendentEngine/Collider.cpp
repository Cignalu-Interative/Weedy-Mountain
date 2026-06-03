#include "pch.h"
#include "Collider.h"
#include "Entity.h"

bool Collider::CanCollideWith(const Collider* p_other) const
{
    return (m_mask & p_other->m_layer) != 0 && (p_other->m_mask & m_layer) != 0;
}

void Collider::SyncWithOwner()
{
    if (m_owner == nullptr)
        return;

    // Position monde = position centre de l'owner + offset relatif
    Vector2f ownerCenter = m_owner->GetPosition({ 0.5f, 0.5f });
    SetPosition({ ownerCenter.GetX() + m_offset.GetX(), ownerCenter.GetY() + m_offset.GetY() });
}