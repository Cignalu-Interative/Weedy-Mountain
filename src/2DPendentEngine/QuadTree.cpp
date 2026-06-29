#include "pch.h"
#include "QuadTree.h"

QuadTree::QuadTree(const AABB& p_bounds, int p_depth) : m_bounds(p_bounds), m_depth(p_depth)
{
    m_objects.reserve(QT_MAX_OBJECTS);
}

QuadTree::~QuadTree()
{
    for (QuadTree* child : m_children)
        delete child;
}

void QuadTree::Clear()
{
    m_objects.clear();

    if (m_divided)
    {
        for (QuadTree* child : m_children)
        {
            if (child) 
                child->Clear();
        }
    }
}

AABB QuadTree::GetChildBounds(int i) const
{
    float midX = (m_bounds.minX + m_bounds.maxX) * 0.5f;
    float midY = (m_bounds.minY + m_bounds.maxY) * 0.5f;

    switch (i)
    {
    case 0: 
        return { m_bounds.minX, m_bounds.minY, midX, midY }; // NW
    case 1: 
        return { midX, m_bounds.minY, m_bounds.maxX, midY }; // NE
    case 2:
        return { m_bounds.minX, midY, midX, m_bounds.maxY }; // SW
    case 3:
        return { midX, midY, m_bounds.maxX, m_bounds.maxY }; // SE
    default:
        return m_bounds;
    }
}

void QuadTree::Subdivide()
{
    for (int i = 0; i < 4; ++i)
    {
        if (m_children[i] == nullptr)
            m_children[i] = new QuadTree(GetChildBounds(i), m_depth + 1);
        else
            m_children[i]->m_bounds = GetChildBounds(i); 
    }
    m_divided = true;
}

int QuadTree::GetChildIndex(const AABB& p_aabb) const
{
    float midX = (m_bounds.minX + m_bounds.maxX) * 0.5f;
    float midY = (m_bounds.minY + m_bounds.maxY) * 0.5f;

    bool inLeft = p_aabb.maxX <= midX;
    bool inRight = p_aabb.minX >= midX;
    bool inTop = p_aabb.maxY <= midY;
    bool inBot = p_aabb.minY >= midY;

    if (inLeft && inTop) 
        return 0; // NW
    if (inRight && inTop) 
        return 1; // NE
    if (inLeft && inBot) 
        return 2; // SW
    if (inRight && inBot) 
        return 3; // SE

    return -1;
}

void QuadTree::Insert(Collider* p_collider)
{
    const AABB& aabb = p_collider->GetCachedAABB();

    if (Overlaps(aabb) == false)
        return;

    if (m_divided)
    {
        int idx = GetChildIndex(aabb);

        if (idx >= 0)
        {
            m_children[idx]->Insert(p_collider);
            return;
        }

        m_objects.push_back(p_collider);
        return;
    }

    m_objects.push_back(p_collider);

    if ((int)m_objects.size() > QT_MAX_OBJECTS && m_depth < QT_MAX_DEPTH)
    {
        Subdivide();

        auto it = m_objects.begin();

        while (it != m_objects.end())
        {
            int idx = GetChildIndex((*it)->GetCachedAABB());

            if (idx >= 0)
            {
                m_children[idx]->Insert(*it);
                it = m_objects.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void QuadTree::Query(const AABB& p_query, std::vector<Collider*>& p_candidates) const
{
    if (Overlaps(p_query) == false)
        return;

    for (Collider* c : m_objects)
        p_candidates.push_back(c);

    if (m_divided)
    {
        for (const QuadTree* child : m_children)
        {
            if (child)
            {
                child->Query(p_query, p_candidates);
            }
        }
    }
}