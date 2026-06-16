#include "pch.h"
#include "CollisionManager.h"
#include "QuadTree.h"
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
    Entity* owner = p_collider->GetOwner();
    PhysicsBody* body = owner ? owner->GetPhysicsBody() : nullptr;

    if (body == nullptr || body->GetBodyType() == ShapeBodyPhysics::Static)
        m_staticColliders.push_back(p_collider);
    else
        m_dynamicColliders.push_back(p_collider);
}

void CollisionManager::UnregisterCollider(Collider* p_collider)
{

    m_staticColliders.erase(std::remove(m_staticColliders.begin(), m_staticColliders.end(), p_collider), m_staticColliders.end());
    m_dynamicColliders.erase(std::remove(m_dynamicColliders.begin(), m_dynamicColliders.end(), p_collider), m_dynamicColliders.end());

    auto removePairs = [&](ColliderPairSet& p_set)
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
    for (Collider* c : m_staticColliders)
        c->SyncWithOwner();

    for (Collider* c : m_dynamicColliders)
        c->SyncWithOwner();
}

void CollisionManager::RebuildQuadTree()
{
    if (!m_quadTree)
        m_quadTree = std::make_unique<QuadTree>(m_worldBounds);
    else
        m_quadTree->Clear();

    for (Collider* c : m_staticColliders)
        m_quadTree->Insert(c);

    for (Collider* c : m_dynamicColliders)
        m_quadTree->Insert(c);
}

void CollisionManager::TestPair(Collider* a, Collider* b)
{
    if (a->GetOwner() == b->GetOwner())
        return;

    if (a->CanCollideWith(b) == false)
        return;

    // Broad-phase AABB (les AABB sont deja calcules dans SyncAll)
    if (OverlapAABB(a->GetCachedAABB(), b->GetCachedAABB()) == false)
        return;

    // Narrow-phase : calcul du manifold complet
    CollisionManifold manifold = Collision::ComputeManifold(*a, *b);

    if (manifold.valid == false)
        return;

    ColliderPair pair = MakePair(a, b);
    m_currentPairs.insert(pair);

    Entity* ownerA = a->GetOwner();
    Entity* ownerB = b->GetOwner();

    if (a->IsTrigger() == false && b->IsTrigger() == false)
    {
        PhysicsManager::Get()->Repulse(ownerA, ownerB, manifold);
        PhysicsManager::Get()->ResolveCollision(ownerA, ownerB, manifold);
    }

    if (m_previousPairs.count(pair) == 0)
    {
        if (ownerA) ownerA->OnCollisionEnter(a, b, manifold.sideA);
        if (ownerB) ownerB->OnCollisionEnter(b, a, manifold.sideB);
    }
    else
    {
        if (ownerA) ownerA->OnCollisionStay(a, b, manifold.sideA);
        if (ownerB) ownerB->OnCollisionStay(b, a, manifold.sideB);
    }
}

void CollisionManager::Update()
{
    m_currentPairs.clear();
    m_currentPairs.reserve(m_previousPairs.size() + 64);

    RebuildQuadTree();

    std::vector<Collider*> candidates;
    candidates.reserve(64);

    ColliderPairSet alreadyTested;
    alreadyTested.reserve(m_dynamicColliders.size() * 4);

    for (Collider* dyn : m_dynamicColliders)
    {
        candidates.clear();
        m_quadTree->Query(dyn->GetCachedAABB(), candidates);

        for (Collider* candidate : candidates)
        {
 
            if (candidate == dyn)
                continue;

            ColliderPair pair = MakePair(dyn, candidate);

            if (alreadyTested.insert(pair).second == false)
                continue; 

            TestPair(dyn, candidate);
        }
    }

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

    m_previousPairs = std::move(m_currentPairs);
}

void CollisionManager::Reset()
{
    m_staticColliders.clear();
    m_dynamicColliders.clear();
    m_currentPairs.clear();
    m_previousPairs.clear();

    if (m_quadTree)
        m_quadTree->Clear();
}