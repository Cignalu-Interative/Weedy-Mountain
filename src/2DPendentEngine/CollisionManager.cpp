#include "pch.h"
#include "CollisionManager.h"
#include "Entity.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include <algorithm>

CollisionManager* CollisionManager::Get()
{
    static CollisionManager instance;
    return &instance;
}

ColliderPair CollisionManager::MakePair(Collider* a, Collider* b) const
{
    // Ordre canonique pour que (A,B) == (B,A)
    return (a < b) ? ColliderPair(a, b) : ColliderPair(b, a);
}

void CollisionManager::RegisterCollider(Collider* p_collider)
{
    m_colliders.push_back(p_collider);
}

void CollisionManager::UnregisterCollider(Collider* p_collider)
{
    m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), p_collider), m_colliders.end());

    auto removePairs = [&](std::set<ColliderPair>& p_set)
        {
            for (auto it = p_set.begin(); it != p_set.end(); )
            {
                if (it->first == p_collider || it->second == p_collider)
                    it = p_set.erase(it);
                else
                    ++it;
            }
        };

    removePairs(m_currentPairs);
    removePairs(m_previousPairs);
}

bool CollisionManager::Overlaps(Collider* a, Collider* b) const
{
    // Rectangle vs Rectangle
    if (a->GetShape() == Shape::Rectangle && b->GetShape() == Shape::Rectangle)
    {
        Vector2f posA = a->GetPosition({ 0.f, 0.f });
        Vector2i dimA = a->GetDimension();
        Vector2f posB = b->GetPosition({ 0.f, 0.f });
        Vector2i dimB = b->GetDimension();

        return !(posA.GetX() + dimA.GetX() < posB.GetX() ||
            posB.GetX() + dimB.GetX() < posA.GetX() ||
            posA.GetY() + dimA.GetY() < posB.GetY() ||
            posB.GetY() + dimB.GetY() < posA.GetY());
    }

    // Circle vs Circle
    if (a->GetShape() == Shape::Circle && b->GetShape() == Shape::Circle)
    {
        Vector2f ca = a->GetPosition({ 0.5f, 0.5f });
        Vector2f cb = b->GetPosition({ 0.5f, 0.5f });
        float ra = a->GetDimension().GetX() / 2.f;
        float rb = b->GetDimension().GetX() / 2.f;
        float dx = ca.GetX() - cb.GetX();
        float dy = ca.GetY() - cb.GetY();
        return (dx * dx + dy * dy) < (ra + rb) * (ra + rb);
    }

    // Rectangle vs Circle (on délègue au cercle)
    Collider* rect = (a->GetShape() == Shape::Rectangle) ? a : b;
    Collider* circle = (a->GetShape() == Shape::Circle) ? a : b;

    Vector2f cr = circle->GetPosition({ 0.5f, 0.5f });
    float rad = circle->GetDimension().GetX() / 2.f;
    Vector2f rp = rect->GetPosition({ 0.f, 0.f });
    Vector2i rd = rect->GetDimension();

    float testX = std::max(rp.GetX(), std::min(cr.GetX(), rp.GetX() + rd.GetX()));
    float testY = std::max(rp.GetY(), std::min(cr.GetY(), rp.GetY() + rd.GetY()));
    float dx = cr.GetX() - testX;
    float dy = cr.GetY() - testY;
    return (dx * dx + dy * dy) < (rad * rad);
}

CollisionSide CollisionManager::ComputeSide(Collider* self, Collider* other) const
{
    // Overlap minimal sur chaque axe -> détermine la face
    Vector2f posS = self->GetPosition({ 0.5f, 0.5f });
    Vector2f posO = other->GetPosition({ 0.5f, 0.5f });
    Vector2i dimS = self->GetDimension();
    Vector2i dimO = other->GetDimension();

    float overlapX = (dimS.GetX() / 2.f + dimO.GetX() / 2.f) - std::abs(posS.GetX() - posO.GetX());
    float overlapY = (dimS.GetY() / 2.f + dimO.GetY() / 2.f) - std::abs(posS.GetY() - posO.GetY());

    if (overlapX < overlapY)
        return (posS.GetX() < posO.GetX()) ? CollisionSide::RIGHT : CollisionSide::LEFT;
    else
        return (posS.GetY() < posO.GetY()) ? CollisionSide::BOTTOM : CollisionSide::TOP;
}

void CollisionManager::SyncAll()
{
    for (Collider* c : m_colliders)
        c->SyncWithOwner();
}

void CollisionManager::Update()
{
    m_currentPairs.clear();

    // Détection O(n²) sur tous les colliders enregistrés
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
            Collider* a = m_colliders[i];
            Collider* b = m_colliders[j];

            // Pas de collision entre colliders du même owner
            if (a->GetOwner() == b->GetOwner())
                continue;

            if (a->CanCollideWith(b) == false)
                continue;

            if (Overlaps(a, b) == false)
                continue;

            ColliderPair pair = MakePair(a, b);
            m_currentPairs.insert(pair);

            Entity* ownerA = a->GetOwner();
            Entity* ownerB = b->GetOwner();

            if (m_previousPairs.count(pair) == 0)
            {
                CollisionSide sideA = ComputeSide(a, b);
                CollisionSide sideB = ComputeSide(b, a);

                if (ownerA) 
                    ownerA->OnCollisionEnter(a, b, sideA);

                if (ownerB) 
                    ownerB->OnCollisionEnter(b, a, sideB);
            }
            else
            {
                CollisionSide sideA = ComputeSide(a, b);
                CollisionSide sideB = ComputeSide(b, a);

                if (ownerA)
                    ownerA->OnCollisionStay(a, b, sideA);

                if (ownerB)
                    ownerB->OnCollisionStay(b, a, sideB);
            }
        }
    }

    for (const ColliderPair& pair : m_previousPairs)
    {
        if (m_currentPairs.count(pair) == 0)
        {
            Entity* ownerA = pair.first->GetOwner();
            Entity* ownerB = pair.second->GetOwner();
            if (ownerA) ownerA->OnCollisionExit(pair.first, pair.second, CollisionSide::NONE);
            if (ownerB) ownerB->OnCollisionExit(pair.second, pair.first, CollisionSide::NONE);
        }
    }

    m_previousPairs = m_currentPairs;
}