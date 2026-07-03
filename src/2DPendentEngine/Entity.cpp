#include "pch.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Scene.h"
#include "2DPendentLib/Utils.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "CollisionManager.h"
#include "PhysicsManager.h"
#include "SpriteContainer.h"
#include "Animation.h"

Entity::Entity()
{
	m_sprites.reserve(4);
	m_colliders.reserve(4);
}

Entity::~Entity()
{
	for (SpriteContainer* sc : m_sprites)
	{
		delete sc;
	}
	m_sprites.clear();

	for (Collider* c : m_colliders)
	{
		CollisionManager::Get()->UnregisterCollider(c);
		delete c;
	}
	m_colliders.clear();

	if (m_physicsBody != nullptr)
	{
		PhysicsManager::Get()->UnregisterBody(this);
		delete m_physicsBody;
		m_physicsBody = nullptr;
	}
}

void Entity::Initialize(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Render* f_render, Shape p_shape, bool p_isGUI, bool p_animation, const std::string& p_jsonAnimation)
{
	SetDimension(p_dimension);
	SetPosition(p_position);

	SpriteContainer* spriteContainer = AddSprite("default", p_path, p_dimension, 0, { 0.f,0.f }, p_isGUI);

	AddCollider("default", p_dimension, p_shape);

	if (p_animation)
	{
		m_animation = new Animation();
		m_animation->Init(spriteContainer->m_sprite, "default", true, p_jsonAnimation);
	}

	OnInitialize();
}

void Entity::Update()
{
	float deltatime = GetDeltaTime();
	if (m_animation != nullptr)
		m_animation->Update(deltatime);

	OnUpdate();
}

void Entity::Draw(Render* f_render, Window* f_window)
{
	Camera* p_camera = m_scene->GetCamera();

	Vector2i dimensionWindow = f_window->GetWindowSize();

	// Bounding box combin�e de tous les sprites visibles
	bool hasAnySprite = false;
	float minX = FLT_MAX, minY = FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX;

	for (SpriteContainer* sc : m_sprites)
	{
		if (!sc->m_sprite || sc->m_visible == false)
			continue;

		hasAnySprite = true;

		Vector2f tl = sc->m_sprite->GetPosition({ 0.f, 0.f });
		Vector2f br = sc->m_sprite->GetPosition({ 1.f, 1.f });
		minX = std::min(minX, tl.GetX());
		minY = std::min(minY, tl.GetY());
		maxX = std::max(maxX, br.GetX());
		maxY = std::max(maxY, br.GetY());
	}

	// Fallback sur la bounding box de l'entity si aucun sprite
	if (hasAnySprite == false)
	{
		minX = GetPosition({ 0.f, 0.f }).GetX();
		minY = GetPosition({ 0.f, 0.f }).GetY();
		maxX = GetPosition({ 1.f, 1.f }).GetX();
		maxY = GetPosition({ 1.f, 1.f }).GetY();
	}

	Vector2f screenTL = p_camera->WorldToScreen(Vector2f{ minX, minY });
	Vector2f screenBR = p_camera->WorldToScreen(Vector2f{ maxX, maxY });

	if (GetIsGUI() == false && (screenBR.GetX() < 0 || screenTL.GetX() > dimensionWindow.GetX() || screenBR.GetY() < 0 || screenTL.GetY() > dimensionWindow.GetY()))
		return;

	// Dessin dans l'ordre zOrder 
	for (SpriteContainer* sc : m_sprites)
	{
		if (!sc->m_sprite || sc->m_visible == false)
			continue;

		sc->m_sprite->Draw(f_render, f_window, p_camera);
	}

	// It�re sur tous les colliders
	for (Collider* c : m_colliders)
	{
		if (c->IsShow() == false)
			continue;

		switch (c->GetShape())
		{
		case Shape::Circle:
			static_cast<CircleCollider*>(c)->Draw(f_render, f_window, p_camera);
			break;
		case Shape::Rectangle:
			static_cast<RectangleCollider*>(c)->Draw(f_render, f_window, p_camera);
			break;
		}
	}
	OnDraw(f_render, f_window);
}

void Entity::Destroy()
{
	OnDestroy();
	GetScene()->RemoveEntity(this);
}

void Entity::MoveKinematic()
{
	if (m_physicsBody == nullptr)
		return;

	if (m_physicsBody->GetBodyType() != ShapeBodyPhysics::Kinematic)
		return;

	if (m_direction.GetX() != 0.0f || m_direction.GetY() != 0.0f)
	{
		Vector2f dirNorm = Utils::NormalizeVector(m_direction);
		m_physicsBody->SetLinearVelocity({
			dirNorm.GetX() * m_speed,
			dirNorm.GetY() * m_speed
			});
	}
	else
	{
		m_physicsBody->SetLinearVelocity({ 0.0f, 0.0f });
	}

	if (m_resetDirection)
		m_direction = { 0.0f, 0.0f };
}

void Entity::InitialisePhysicsBody(ShapeBodyPhysics p_shapeBodyPhysics)
{
	if (!m_physicsBody)
	{
		m_physicsBody = new PhysicsBody(p_shapeBodyPhysics);
		PhysicsManager::Get()->RegisterBody(this);
	}
	else
	{
		m_physicsBody->SetBodyType(p_shapeBodyPhysics);
	}

	SetRigidBody(true);
}

float Entity::GetDeltaTime() const
{
	if (m_scene == nullptr)
	{
		return 0.0f;
	}
	return m_scene->GetDeltaTime();
}

void Entity::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Transformable::SetPosition(p_position, p_anchor);

	SyncColliders();
	SyncSprites();
}

void Entity::SetDimension(Vector2i p_newDimension)
{
	Vector2i oldDim = GetDimension();
	Transformable::SetDimension(p_newDimension);

	// Ratio pour scaler tous les sprites & colliders proportionnellement
	float ratioX = (oldDim.GetX() > 0) ? (float)p_newDimension.GetX() / oldDim.GetX() : 1.f;
	float ratioY = (oldDim.GetY() > 0) ? (float)p_newDimension.GetY() / oldDim.GetY() : 1.f;

	for (SpriteContainer* sc : m_sprites)
	{
		if (!sc->m_sprite)
			continue;

		Vector2i oldSpriteDim = sc->m_sprite->GetDimension();
		sc->m_sprite->SetDimension({ static_cast<int>(oldSpriteDim.GetX() * ratioX), static_cast<int>(oldSpriteDim.GetY() * ratioY) });
	}

	for (Collider* col : m_colliders)
	{
		Vector2i oldSpriteDim = col->GetDimension();
		col->SetDimension({ static_cast<int>(oldSpriteDim.GetX() * ratioX), static_cast<int>(oldSpriteDim.GetY() * ratioY) });
	}
}

void Entity::SetAngle(float p_angle)
{
	Transformable::SetAngle(p_angle);

	for (Collider* c : m_colliders)
		c->SetAngle(p_angle);

	for (SpriteContainer* sc : m_sprites)
	{
		if (sc->m_sprite)
			sc->m_sprite->SetAngle(p_angle);
	}
}

void Entity::Move(Vector2f p_translation)
{
	Transformable::Move(p_translation);

	SyncColliders();
	SyncSprites();
}

void Entity::SetIsGUI(bool p_isGUI)
{
	for (SpriteContainer* sc : m_sprites)
	{
		if (sc->m_sprite)
			sc->m_sprite->SetIsGUI(p_isGUI);
	}
}

bool Entity::GetIsGUI() const
{
	for (SpriteContainer* sc : m_sprites)
	{
		if (sc->m_sprite)
		{
			if (sc->m_sprite->IsGUI() == true)
				return true;
		}
	}

	return false;
}

Collider* Entity::AddCollider(const std::string& p_id, Vector2i p_dimension, Shape p_shape, Vector2f p_offset, bool p_isTrigger, uint32_t p_layer, uint32_t p_mask)
{
	Collider* col = nullptr;

	if (p_shape == Shape::Rectangle)
	{
		auto* rc = new RectangleCollider();
		rc->Initialize(GetPosition({ 0.5f, 0.5f }), p_dimension);
		col = rc;
	}
	else if (p_shape == Shape::Circle)
	{
		auto* cc = new CircleCollider();
		cc->Initialize(GetPosition({ 0.5f, 0.5f }), p_dimension);
		col = cc;
	}

	col->SetID(p_id);
	col->SetOffset(p_offset);
	col->SetIsTrigger(p_isTrigger);
	col->SetLayer(p_layer);
	col->SetMask(p_mask);
	col->SetOwner(this);
	col->SyncWithOwner();

	m_colliders.push_back(col);
	CollisionManager::Get()->RegisterCollider(col);

	return col;
}

Collider* Entity::GetCollider(const std::string& p_id) const
{
	for (Collider* c : m_colliders)
	{
		if (c->GetID() == p_id)
			return c;
	}
	return nullptr;
}

Collider* Entity::GetCollider() const
{
	return m_colliders.empty() ? nullptr : m_colliders[0];
}

void Entity::RemoveCollider(const std::string& p_id)
{
	for (auto it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		if ((*it)->GetID() == p_id)
		{
			CollisionManager::Get()->UnregisterCollider(*it);
			delete* it;
			m_colliders.erase(it);
			return;
		}
	}
}

void Entity::SyncColliders()
{
	for (Collider* c : m_colliders)
		c->SyncWithOwner();
}

void Entity::ShowCollider(bool p_show)
{
	for (Collider* c : m_colliders)
		c->SetShow(p_show);
}

void Entity::ShowCollider(const std::string& p_id, bool p_show)
{
	Collider* c = GetCollider(p_id);
	if (c) c->SetShow(p_show);
}

SpriteContainer* Entity::AddSprite(const std::string& p_id, const std::string& p_texturePath, Vector2i p_dimension, int p_zOrder, Vector2f p_offset, bool p_isGUI)
{
	SpriteContainer* sc = new SpriteContainer();
	sc->m_id = p_id;
	sc->m_offset = p_offset;
	sc->m_zOrder = p_zOrder;

	sc->m_sprite = new Sprite();
	sc->m_sprite->Initialise(p_texturePath, GetPosition(), p_dimension, Application::Get()->GetRender());
	sc->m_sprite->SetIsGUI(p_isGUI);

	// Insertion tri�e par zOrder
	auto it = std::lower_bound(m_sprites.begin(), m_sprites.end(), sc, [](const SpriteContainer* a, const SpriteContainer* b)
		{
			return a->m_zOrder < b->m_zOrder;
		});
	m_sprites.insert(it, sc);

	SyncSprites();
	return sc;
}

void Entity::SetSprite(const std::string& p_texturePath)
{
	SetSprite("default", p_texturePath);
}

void Entity::SetSprite(const std::string& p_id, const std::string& p_texturePath)
{
	SpriteContainer* sc = GetSpriteContainer(p_id);
	if (sc && sc->m_sprite)
		sc->m_sprite->ChangeTexture(p_texturePath);
}

SpriteContainer* Entity::GetSpriteContainer(const std::string& p_id) const
{
	for (SpriteContainer* sc : m_sprites)
	{
		if (sc->m_id == p_id)
			return sc;
	}
	return nullptr;
}

SpriteContainer* Entity::GetSpriteContainer() const
{
	return m_sprites.empty() ? nullptr : m_sprites[0];
}

Sprite* Entity::GetSprite(const std::string& p_id) const
{
	SpriteContainer* sc = GetSpriteContainer(p_id);
	return sc ? sc->m_sprite : nullptr;
}

Sprite* Entity::GetSprite() const
{
	SpriteContainer* sc = GetSpriteContainer("default");
	return sc ? sc->m_sprite : nullptr;
}

void Entity::RemoveSprite(const std::string& p_id)
{
	for (auto it = m_sprites.begin(); it != m_sprites.end(); ++it)
	{
		if ((*it)->m_id == p_id)
		{
			delete* it;
			m_sprites.erase(it);
			return;
		}
	}
}

void Entity::SetSpriteVisible(const std::string& p_id, bool p_visible)
{
	SpriteContainer* sc = GetSpriteContainer(p_id);

	if (sc)
		sc->m_visible = p_visible;
}

void Entity::SetAllSpritesVisible(bool p_visible)
{
	for (SpriteContainer* sc : m_sprites)
		sc->m_visible = p_visible;
}

void Entity::SyncSprites()
{
	Vector2f center = GetPosition({ 0.5f, 0.5f });
	for (SpriteContainer* sc : m_sprites)
	{
		if (!sc->m_sprite)
			continue;

		sc->m_sprite->SetPosition({ center.GetX() + sc->m_offset.GetX(), center.GetY() + sc->m_offset.GetY() });
	}
}

void Entity::SetIsVisible(bool p_isVisible)
{
	m_isVisible = p_isVisible;

	SetAllSpritesVisible(p_isVisible);
	//ShowCollider(p_isVisible);
}

void Entity::SetAnimationByName(const std::string& p_nameAnimation)
{
	if (m_animation == nullptr)
		return;
	m_animation->SetAnimationByName(p_nameAnimation);
}