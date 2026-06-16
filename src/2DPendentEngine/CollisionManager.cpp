#include "pch.h"
#include "CollisionManager.h"
#include "PhysicsManager.h"
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

void CollisionManager::SyncAll()
{
    for (Collider* c : m_colliders)
        c->SyncWithOwner();
}

void CollisionManager::Update()
{
    m_currentPairs.clear();

    // D�tection O(n�) sur tous les colliders enregistr�s
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
            Collider* a = m_colliders[i];
            Collider* b = m_colliders[j];

            // Pas de collision entre colliders du m�me owner
            if (a->GetOwner() == b->GetOwner())
                continue;

            if (a->CanCollideWith(b) == false)
                continue;

            CollisionManifold manifold = Collision::ComputeManifold(*a, *b);

            if (manifold.valid == false)
                continue;

            ColliderPair pair = MakePair(a, b);
            m_currentPairs.insert(pair);

            Entity* ownerA = a->GetOwner();
            Entity* ownerB = b->GetOwner();

            // R�solution physique uniquement pour les colliders non-trigger
            if (a->IsTrigger() == false && b->IsTrigger() == false)
            {
                PhysicsManager::Get()->Repulse(ownerA, ownerB, manifold);
                PhysicsManager::Get()->ResolveCollision(ownerA, ownerB, manifold);
            }

            // Callback Enter / Stay
            if (m_previousPairs.count(pair) == 0)
            {
                if (ownerA) 
                    ownerA->OnCollisionEnter(a, b, manifold.sideA);

                if (ownerB) 
                    ownerB->OnCollisionEnter(b, a, manifold.sideB);
            }
            else
            {
                if (ownerA)
                    ownerA->OnCollisionStay(a, b, manifold.sideA);

                if (ownerB)
                    ownerB->OnCollisionStay(b, a, manifold.sideB);
            }
        }
    }

    // Callback Exit
    for (const ColliderPair& pair : m_previousPairs)
    {
        if (m_currentPairs.count(pair) == 0)
        {
            Entity* ownerA = pair.first->GetOwner();
            Entity* ownerB = pair.second->GetOwner();

            if (ownerA)
                ownerA->OnCollisionExit(pair.first, pair.second, CollisionSide::NONE);

            if (ownerB)
                ownerB->OnCollisionExit(pair.second, pair.first, CollisionSide::NONE);
        }
    }

    m_previousPairs = m_currentPairs;
}