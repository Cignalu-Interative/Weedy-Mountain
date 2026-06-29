#pragma once
#include <vector>
#include <array>
#include "Collider.h"

// Nombre max d'entites dans un noeud avant subdivision
static constexpr int QT_MAX_OBJECTS = 8;
// Profondeur max de recursion (evite la subdivision infinie si entites superposees)
static constexpr int QT_MAX_DEPTH = 6;

class QuadTree
{
public:
    QuadTree(const AABB& p_bounds, int p_depth = 0);
    ~QuadTree();

    void Clear();

    void Insert(Collider* p_collider);

    void Query(const AABB& p_query, std::vector<Collider*>& p_candidates) const;

private:
    AABB m_bounds;        
    int m_depth;          

    std::vector<Collider*> m_objects;

    // Les 4 enfants : NW, NE, SW, SE (nullptr si pas encore subdivise)
    std::array<QuadTree*, 4> m_children = { nullptr, nullptr, nullptr, nullptr };

    bool m_divided = false;

    // Subdivise ce noeud en 4 enfants et redistribue les objets
    void Subdivide();

    // Retourne l'index de l'enfant contenant completement p_aabb (-1 si aucun)
    int GetChildIndex(const AABB& p_aabb) const;

    // Calcule les bounds du quadrant i (0=NW 1=NE 2=SW 3=SE)
    AABB GetChildBounds(int i) const;

    // Verifie si p_aabb est entierement contenu dans m_bounds
    inline bool Contains(const AABB& p_aabb) const { return p_aabb.minX >= m_bounds.minX && p_aabb.maxX <= m_bounds.maxX && p_aabb.minY >= m_bounds.minY && p_aabb.maxY <= m_bounds.maxY; }

    // Verifie si p_aabb chevauche m_bounds
    inline bool Overlaps(const AABB& p_aabb) const { return (p_aabb.maxX < m_bounds.minX || p_aabb.minX > m_bounds.maxX || p_aabb.maxY < m_bounds.minY || p_aabb.minY > m_bounds.maxY) == false; }
};