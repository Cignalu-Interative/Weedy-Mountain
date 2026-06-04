#include "pch.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Scene.h"
#include "2DPendentLib/Utils.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"

Entity::~Entity()
{
	delete m_sprite;
	delete m_collider;
	delete m_physicsBody;
}

void Entity::Initialize(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Render* f_render, Shape p_shape, bool p_isGUI)
{
	SetDimension(p_dimension);
	SetPosition(p_position);

	Sprite* sprite = new Sprite();
	sprite->Initialise(p_path, p_position, p_dimension, f_render);
	sprite->SetIsGUI(p_isGUI);
	m_sprite = sprite;

	switch (p_shape)
	{
	case Shape::Circle:
	{
		CircleCollider* collider = new CircleCollider();
		collider->Initialize(p_position, p_dimension);
		m_collider = collider;
		break;
	}
	case Shape::Rectangle:
	{
		RectangleCollider* collider = new RectangleCollider();
		collider->Initialize(p_position, p_dimension);
		m_collider = collider;
		break;
	}
	}

	OnInitialize();
}

void Entity::Update()
{
	//OnEvent();

	/*float deltatime = GetDeltaTime();
	UpdateMovement(deltatime);*/

	OnUpdate();
}

void Entity::Draw(Render* f_render, Window* f_window)
{
	Camera* p_camera = m_scene->GetCamera();

	Vector2i dimensionWindow = f_window->GetWindowSize();

	Vector2f screenPosTopLeft = p_camera->WorldToScreen(GetPosition({ 0.0f, 0.0f }));
	Vector2f screenPosBottomRight = p_camera->WorldToScreen(GetPosition({ 1.0f, 1.0f }));

	if (screenPosBottomRight.GetX() < 0 || screenPosTopLeft.GetX() > dimensionWindow.GetX() ||
		screenPosBottomRight.GetY() < 0 || screenPosTopLeft.GetY() > dimensionWindow.GetY())
	{
		//std::cerr << "no draw" << std::endl;
		return;
	}

	if (m_sprite)
	{
		m_sprite->Draw(f_render, f_window, p_camera);
	}
	if (m_collider)
	{
		Shape shape = m_collider->GetShape();
		switch (shape)
		{
		case Shape::Circle:
		{
			CircleCollider* collider = GetCollider<CircleCollider>();
			collider->Draw(f_render, f_window, p_camera);
			break;
		}
		case Shape::Rectangle:
		{
			RectangleCollider* collider = GetCollider<RectangleCollider>();
			collider->Draw(f_render, f_window, p_camera);
			break;
		}
		}
	}
}

void Entity::Destroy()
{
	m_toDestroy = true;

	OnDestroy();
}

void Entity::UpdateMovement_Forces(float p_deltaTime)
{
	if (p_deltaTime == 0.0f || !m_physicsBody)
		return;

	if (m_direction.GetX() != 0.0f || m_direction.GetY() != 0.0f)
	{
		Vector2f dirNorm = Utils::NormalizeVector(m_direction);
		m_physicsBody->SetLinearVelocity({ dirNorm.GetX() * m_speed * m_PIXELS_PER_METER, m_physicsBody->GetLinearVelocity().GetY() });
	}
	m_physicsBody->IntegrateForces(p_deltaTime);
}

void Entity::UpdateMovement_Velocity(float p_deltaTime)
{
	if (p_deltaTime == 0.0f)
		return;

	if (m_physicsBody)
	{
		/*std::cout << "AVANT IntegrateVelocity, physicsBody pos="
			<< m_physicsBody->GetPosition().GetX() << " "
			<< m_physicsBody->GetPosition().GetY() << std::endl;*/

		m_physicsBody->IntegrateVelocity(p_deltaTime);

		/*std::cout << "APRES IntegrateVelocity, physicsBody pos="
			<< m_physicsBody->GetPosition().GetX() << " "
			<< m_physicsBody->GetPosition().GetY() << std::endl;*/

		Vector2f newPosition = m_physicsBody->GetPosition();
		//std::cout << "newPos depuis physicsBody=" << newPosition.GetX() << " " << newPosition.GetY() << std::endl;
		SetPosition(newPosition);
		float newAngle = m_physicsBody->GetAngle();
		SetAngle(newAngle);

		/*std::cout << "position entity apres SetPosition="
			<< GetPosition().GetX() << " " << GetPosition().GetY() << std::endl;*/
	}
	else
	{
		if (m_speed == 0.0f || m_direction == Vector2<float>{ 0.f, 0.f })
			return;
		float distance = p_deltaTime * m_speed * m_PIXELS_PER_METER;
		Vector2f dirNorm = Utils::NormalizeVector(m_direction);
		Vector2f translation = dirNorm * distance;
		Move(translation);
	}

	const float windowWidth = static_cast<float>(m_scene->GetWindowWidth());
	const float windowHeight = static_cast<float>(m_scene->GetWindowHeight());

	const float PosLeft = GetPosition({ 0.0f, 0.5f }).GetX();
	const float PosRight = GetPosition({ 1.0f, 0.5f }).GetX();
	const float PosTop = GetPosition({ 0.5f, 0.0f }).GetY();
	const float PosBottom = GetPosition({ 0.5f, 1.0f }).GetY();

	const float centerX = GetPosition().GetX();
	const float centerY = GetPosition().GetY();

	const bool outX = PosLeft < 0.0f || PosRight > windowWidth;
	const bool outY = PosTop < 0.0f || PosBottom > windowHeight;

	if (outX || outY)
	{
		if (m_clampToWindow)
		{
			if (PosLeft < 0.0f)
			{
				SetPosition({ 0.0f, centerY }, { 0.0f, 0.5f });
			}
			else if (PosRight > windowWidth)
			{
				SetPosition({ windowWidth, centerY }, { 1.0f, 0.5f });
			}

			if (PosTop < 0.0f)
			{
				SetPosition({ centerX, 0.0f }, { 0.5f, 0.0f });
			}
			else if (PosBottom > windowHeight)
			{
				SetPosition({ centerX, windowHeight }, { 0.5f, 1.0f });
			}
			if (m_physicsBody)
			{
				// Si l'entité est sortie horizontalement, on met la vélocité horizontale à 0, sinon on la conserve
					// Si l'entité est sortie verticalement, on met la vélocité verticale à 0, sinon on la conserve
				m_physicsBody->SetLinearVelocity({
					outX ? 0.0f : m_physicsBody->GetLinearVelocity().GetX(),
					outY ? 0.0f : m_physicsBody->GetLinearVelocity().GetY() }
					);
			}
		}
		else
		{
			OnOutBorder();
		}
	}

	Vector2f actualPosition = GetPosition();
	SetPosition(actualPosition);

	if (m_resetDirection)
	{
		m_direction = { 0.0f, 0.0f };
	}
}
//void Entity::UpdateMovement(float p_deltaTime)
//{
//	if (p_deltaTime == 0.0f)
//		return;
//
//	Vector2f dirNorm;
//	if (m_physicsBody)
//	{
//		if (m_direction.GetX() != 0.0f || m_direction.GetY() != 0.0f)
//		{
//			dirNorm = Utils::NormalizeVector(m_direction);
//			m_physicsBody->SetLinearVelocity({ dirNorm.GetX() * m_speed * m_PIXELS_PER_METER, m_physicsBody->GetLinearVelocity().GetY() });
//		}
//		m_physicsBody->IntegrateForces(p_deltaTime);
//		// To Do faire la gestion des collisions dans application
//		m_physicsBody->IntegrateVelocity(p_deltaTime);
//		Vector2f newPosition = m_physicsBody->GetPosition();
//		SetPosition(newPosition);	
//		float newAngle = m_physicsBody->GetAngle();
//		SetAngle(newAngle);
//	}
//	else
//	{
//		if (m_speed == 0.0f || m_direction == Vector2<float>{0.f, 0.f})
//			return;
//
//		float distance = p_deltaTime * m_speed * m_PIXELS_PER_METER;
//		dirNorm = Utils::NormalizeVector(m_direction);
//		Vector2f translation = dirNorm * distance;
//		Move(translation);
//	}
//
//	const float windowWidth = static_cast<float>(m_scene->GetWindowWidth());
//	const float windowHeight = static_cast<float>(m_scene->GetWindowHeight());
//
//	const float PosLeft = GetPosition({ 0.0f, 0.5f }).GetX();
//	const float PosRight = GetPosition({ 1.0f, 0.5f }).GetX();
//	const float PosTop = GetPosition({ 0.5f, 0.0f }).GetY();
//	const float PosBottom = GetPosition({ 0.5f, 1.0f }).GetY();
//
//	const float centerX = GetPosition().GetX();
//	const float centerY = GetPosition().GetY();
//
//	const bool outX = PosLeft < 0.0f || PosRight > windowWidth;
//	const bool outY = PosTop < 0.0f || PosBottom > windowHeight;
//
//	if (outX || outY)
//	{
//		if (m_clampToWindow)
//		{
//			if (PosLeft < 0.0f)
//			{
//				SetPosition({ 0.0f, centerY }, { 0.0f, 0.5f });
//			}
//			else if (PosRight > windowWidth)
//			{
//				SetPosition({ windowWidth, centerY }, { 1.0f, 0.5f });
//			}
//
//			if (PosTop < 0.0f)
//			{
//				SetPosition({ centerX, 0.0f }, { 0.5f, 0.0f });
//			}
//			else if (PosBottom > windowHeight)
//			{
//				SetPosition({ centerX, windowHeight }, { 0.5f, 1.0f });
//			}
//			if (m_physicsBody)
//			{
//				// Si l'entité est sortie horizontalement, on met la vélocité horizontale à 0, sinon on la conserve
//					// Si l'entité est sortie verticalement, on met la vélocité verticale à 0, sinon on la conserve
//				m_physicsBody->SetLinearVelocity({
//					outX ? 0.0f : m_physicsBody->GetLinearVelocity().GetX(),
//					outY ? 0.0f : m_physicsBody->GetLinearVelocity().GetY() }
//				);
//			}
//		}
//		else
//		{
//			OnOutBorder();
//		}
//	}
//
//	Vector2f actualPosition = GetPosition();
//	SetPosition(actualPosition);
//	
//	if (m_resetDirection)
//	{
//		m_direction = { 0.0f, 0.0f };
//	}
//}

void Entity::InitialisePhysicsBody(ShapeBodyPhysics p_shapeBodyPhysics)
{
	if (!m_physicsBody)
	{
		PhysicsBody* physicsBody = new PhysicsBody(p_shapeBodyPhysics);
		m_physicsBody = physicsBody;
	}
	m_physicsBody->SetBodyType(p_shapeBodyPhysics);
	m_physicsBody->SetPosition(GetPosition());
	m_physicsBody->SetDimension(GetDimension());
	m_physicsBody->SetAngle(GetAngle());
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

//bool Entity::IsCollidingWith(Entity* p_other)
//{
//	if (m_collider == nullptr || p_other->m_collider == nullptr || p_other == nullptr)
//		return false;
//
//	return m_collider->IsColliding(p_other->m_collider);
//}

void Entity::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Transformable::SetPosition(p_position, p_anchor);
	if (m_collider)
		m_collider->SetPosition(p_position, p_anchor);
	if (m_physicsBody)
		m_physicsBody->SetPosition(p_position, p_anchor);
	if (m_sprite)
		m_sprite->SetPosition(p_position, p_anchor);
}

void Entity::SetDimension(Vector2i p_dimension)
{
	Transformable::SetDimension(p_dimension);
	if (m_collider)
		m_collider->SetDimension(p_dimension);
	if (m_physicsBody)
		m_physicsBody->SetDimension(p_dimension);
	if (m_sprite)
		m_sprite->SetDimension(p_dimension);
}

void Entity::SetAngle(float p_angle)
{
	Transformable::SetAngle(p_angle);
	if (m_collider)
		m_collider->SetAngle(p_angle);
	if (m_physicsBody)
		m_physicsBody->SetAngle(p_angle);
	if (m_sprite)
		m_sprite->SetAngle(p_angle);
}

void Entity::SetIsGUI(bool p_isGUI)
{
	if (m_sprite)
		m_sprite->SetIsGUI(p_isGUI);
}

bool Entity::GetIsGUI() const
{
	if (m_sprite)
	{
		return m_sprite->IsGUI();
	}
	return false;
}

void Entity::Move(Vector2f p_translation)
{
	Transformable::Move(p_translation);
	if (m_collider)
		m_collider->Move(p_translation);
	if (m_physicsBody)
		m_physicsBody->Move(p_translation);
	if (m_sprite)
		m_sprite->Move(p_translation);
}

void Entity::SetIsVisible(bool p_isVisible)
{
	m_isVisible = p_isVisible;
	if (m_collider)
		m_collider->SetShow(p_isVisible);
	if (m_sprite)
		m_sprite->SetIsVisible(p_isVisible);
}