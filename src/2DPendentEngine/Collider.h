#pragma once
#include "2DPendentLib/Transformable.h"
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/Color.h"
#include "2DPendentLib/Shape.h"

enum class CollisionSide { TOP, BOTTOM, LEFT, RIGHT, NONE };

struct AABB
{
    float minX = 0.f, minY = 0.f, maxX = 0.f, maxY = 0.f;
};

class Entity;
class Render;
class Window;
class Camera;

class Collider : public Transformable
{
    uint32_t m_layer = 1;           // layer de ce collider (1 bit = layer 0 par défaut)
    uint32_t m_mask = 0xFFFFFFFF;  // avec quels layers il peut entrer en collision

protected:
    Shape m_shape;
    std::string m_id;
    Vector2f m_offset = { 0.f, 0.f }; // offset relatif au centre de l'entity
    Color m_color = ColorsNamed::Pink;
    bool m_isShow = false;
    bool m_isTrigger = false; // trigger = détecte sans résoudre la physique
    Entity* m_owner = nullptr;

    AABB m_cachedAABB;

public:
    Collider() = default;
    virtual ~Collider() = default;

    virtual void Draw(Render*, Window*, Camera*) {}

    // ID
    const std::string& GetID() const { return m_id; }
    void SetID(const std::string& p_id) { m_id = p_id; }

    //Layer & Mask
    uint32_t GetLayer() const { return m_layer; }
    void SetLayer(uint32_t p_layer) { m_layer = p_layer; }

    uint32_t GetMask() const { return m_mask; }
    void     SetMask(uint32_t p_mask) { m_mask = p_mask; }

    bool CanCollideWith(const Collider* p_other) const;

    // Offset relatif
    Vector2f GetOffset() const { return m_offset; }
    void SetOffset(Vector2f p_offset) { m_offset = p_offset; }

    // Sync position monde depuis la position de l'owner
    void SyncWithOwner();

    // Owner
    Entity* GetOwner() const { return m_owner; }
    void SetOwner(Entity* p_owner) { m_owner = p_owner; }

    Shape GetShape() const { return m_shape; }
    void SetShape(Shape p_shape) { m_shape = p_shape; }

    void SetColor(Color p_color) { m_color = p_color; }
    Color GetColor() const { return m_color; }

    void SetShow(bool p_show) { m_isShow = p_show; }
    bool IsShow() const { return m_isShow; }

    void SetIsTrigger(bool p_trigger) { m_isTrigger = p_trigger; }
    bool IsTrigger() const { return m_isTrigger; }

    const AABB& GetCachedAABB() const { return m_cachedAABB; }
};