#pragma once
#include <vector>
#include <set>
#include "Collider.h"
#include "CollisionManifold.h"

class Entity;

// Paire de colliders identifiée de façon unique (ordre canonique par pointeur)
using ColliderPair = std::pair<Collider*, Collider*>;

struct CollisionInfo
{
    Collider* self = nullptr;
    Collider* other = nullptr;
    CollisionSide side = CollisionSide::NONE;
};

class CollisionManager
{
private:
    // Tous les colliders enregistrés
    std::vector<Collider*> m_colliders;

    // Paires en collision la frame précédente
    std::set<ColliderPair> m_previousPairs;
    // Paires en collision cette frame
    std::set<ColliderPair> m_currentPairs;

    CollisionManager() = default;

    ColliderPair MakePair(Collider* a, Collider* b) const;

public:
    static CollisionManager* Get();

    void RegisterCollider(Collider* p_collider);
    void UnregisterCollider(Collider* p_collider);

    // Appelé une fois par frame par Application
    void Update();

    // Appelé par Application::Update après le SyncWithOwner de toutes les entités
    void SyncAll();
};