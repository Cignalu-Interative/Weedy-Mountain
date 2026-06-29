#pragma once
#include <vector>
#include <unordered_set>
#include "Collider.h"
#include "CollisionManifold.h"

class Entity;
class QuadTree;

// Paire de colliders identifiée de façon unique (ordre canonique par pointeur)
using ColliderPair = std::pair<Collider*, Collider*>;

struct ColliderPairHash
{
    size_t operator()(const ColliderPair& p) const noexcept
    {
        size_t h1 = std::hash<Collider*>{}(p.first);
        size_t h2 = std::hash<Collider*>{}(p.second);
        return h1 ^ (h2 * 2654435761u);
    }
};

using ColliderPairSet = std::unordered_set<ColliderPair, ColliderPairHash>;

struct CollisionInfo
{
    Collider* self = nullptr;
    Collider* other = nullptr;
    CollisionSide side = CollisionSide::NONE;
};

class CollisionManager
{
private:
    std::vector<Collider*> m_colliders;

    std::vector<Collider*> m_staticColliders;
    std::vector<Collider*> m_dynamicColliders;

    // Paires en collision la frame precedente
    ColliderPairSet m_previousPairs;
    // Paires en collision cette frame
    ColliderPairSet m_currentPairs;

    std::unique_ptr<QuadTree> m_quadTree;

    AABB m_worldBounds = { -2000.f, -2000.f, 6000.f, 6000.f };

    CollisionManager() = default;

    ColliderPair MakePair(Collider* a, Collider* b) const;

    inline bool OverlapAABB(const AABB& a, const AABB& b) const
    {
        return !(a.maxX < b.minX || b.maxX < a.minX ||
            a.maxY < b.minY || b.maxY < a.minY);
    }

    void ClassifyColliders();

    void RebuildQuadTree();

    void TestPair(Collider* a, Collider* b);

public:
    static CollisionManager* Get();

    void RegisterCollider(Collider* p_collider);
    void UnregisterCollider(Collider* p_collider);

    // Appelé une fois par frame par Application
    void Update();

    // Appelé par Application::Update après le SyncWithOwner de toutes les entités
    void SyncAll();

    void SetWorldBounds(float minX, float minY, float maxX, float maxY) { m_worldBounds = { minX, minY, maxX, maxY }; }

    void Reset();
};