#include "pch.h"
#include "Collision.h"
#include "2DPendentLib/Shape.h"
#include "Utils.h"
#include "Entity.h"

//bool Collision::AABB(const Rectangle& p_a, const Rectangle& p_b)
//{
//	Vector2f positionA = p_a.GetPosition();
//	Vector2 dimensionA = p_a.GetDimension();
//	Vector2f positionB = p_b.GetPosition();
//	Vector2 dimensionB = p_b.GetDimension();
//	// Vérifie si les rectangles ne se chevauchent pas
//	return !(
//		positionB.GetX() > positionA.GetX() + dimensionA.GetX() ||
//		positionB.GetX() + dimensionB.GetX() < positionA.GetX() ||
//		positionB.GetY() > positionA.GetY() + dimensionA.GetY() ||
//		positionB.GetY() + dimensionB.GetY() < positionA.GetY());
//}
//
//bool Collision::CBC(const Circle& p_a, const Circle& p_b)
//{
//	Vector2f positionA = p_a.GetPosition();
//	Vector2f positionB = p_b.GetPosition();
//
//	float dx = positionA.GetX() - positionB.GetX();
//	float dy = positionA.GetY() - positionB.GetY();
//
//	float distance = std::sqrt(dx * dx + dy * dy);
//
//	// Les cercles se touchent si la distance entre leurs centres est inférieure à la somme de leurs rayons
//	return distance < (p_a.GetRadius() + p_b.GetRadius());
//}
//
//bool Collision::ARC(const Rectangle& p_rect, const Circle& p_circle)
//{
//	Vector2f rectPosition = p_rect.GetPosition();
//	Vector2 rectDimension = p_rect.GetDimension();
//	Vector2f circlePosition = p_circle.GetPosition();
//	int circleR = p_circle.GetRadius();
//
//	float closestX = std::clamp(circlePosition.GetX(), rectPosition.GetX(), rectPosition.GetX() + rectDimension.GetX());
//	float closestY = std::clamp(circlePosition.GetY(), rectPosition.GetY(), rectPosition.GetY() + rectDimension.GetY());
//	float dx = circlePosition.GetX() - closestX;
//	float dy = circlePosition.GetY() - closestY;
//
//	// Le cercle et le rectangle se touchent si la distance entre le centre du cercle et le point le plus proche du rectangle est inférieure au rayon du cercle
//	return (dx * dx + dy * dy) < (circleR * circleR);
//}
//
//bool Collision::IsColliding(const Collider& p_a, const Collider& p_b)
//{
//	Shape shapeA = p_a.GetShape();
//	Shape shapeB = p_b.GetShape();
//
//	// Rectangle vs Rectangle = AABB
//	if (shapeA == Shape::Rectangle && shapeB == Shape::Rectangle)
//	{
//		return AABB(p_a.GetShapeCollider<Rectangle>(), p_b.GetShapeCollider<Rectangle>());
//	}
//	// Circle vs Circle = CBC
//	if (shapeA == Shape::Circle && shapeB == Shape::Circle)
//	{
//		return CBC(p_a.GetShapeCollider<Circle>(), p_b.GetShapeCollider<Circle>());
//	}
//	// Rectangle vs Circle = ARC (les 2 sens)
//	if (shapeA == Shape::Rectangle && shapeB == Shape::Circle)
//	{
//		return ARC(p_a.GetShapeCollider<Rectangle>(), p_b.GetShapeCollider<Circle>());
//	}
//	// Circle vs Rectangle = ARC (les 2 sens)
//	if (shapeA == Shape::Circle && shapeB == Shape::Rectangle)
//	{
//		return ARC(p_b.GetShapeCollider<Rectangle>(), p_a.GetShapeCollider<Circle>());
//	}
//	return false;
//}

// Projette les coins sur un axe et retourne [min, max]
std::pair<float, float> Collision::Project(const std::array<Vector2f, 4>& p_corners, Vector2f p_axis)
{
    float min = p_corners[0].GetX() * p_axis.GetX() + p_corners[0].GetY() * p_axis.GetY();
    float max = min;

    for (int i = 1; i < 4; i++)
    {
        float proj = p_corners[i].GetX() * p_axis.GetX() + p_corners[i].GetY() * p_axis.GetY();
        if (proj < min) min = proj;
        if (proj > max) max = proj;
    }

    return { min, max };
}

bool Collision::Collision_AABB(const Collider& p_rectA, const Collider& p_rectB)
{
	Vector2f positionA = p_rectA.GetPosition({0,0});
	Vector2 dimensionA = p_rectA.GetDimension();

	Vector2f positionB = p_rectB.GetPosition({ 0,0 });
	Vector2 dimensionB = p_rectB.GetDimension();

	return !(
		positionB.GetX() > positionA.GetX() + dimensionA.GetX() ||
		positionB.GetX() + dimensionB.GetX() < positionA.GetX() ||
		positionB.GetY() > positionA.GetY() + dimensionA.GetY() ||
		positionB.GetY() + dimensionB.GetY() < positionA.GetY());
}

bool Collision::Collision_CBC(const Collider& p_circleA, const Collider& p_circleB)
{
Vector2f positionA = p_circleA.GetPosition();
    Vector2f positionB = p_circleB.GetPosition();

    float distance = Utils::Distance(positionA, positionB);
    float sommeDimention = p_circleA.GetDimension().GetX() / 2 + p_circleB.GetDimension().GetX() / 2;
    float carreDimention = sommeDimention * sommeDimention;
    /*float dx = positionA.GetX() - positionB.GetX();
    float dy = positionA.GetY() - positionB.GetY();

    float distance = std::sqrt(dx dx + dy * dy);*/

    return distance < carreDimention; 
}

bool Collision::Collision_ARC(const Collider& p_rect, const Collider& p_circle)
{
	Vector2f circlePosition = p_circle.GetPosition();
	float circleRadius = p_circle.GetDimension().GetX() / 2;

	Vector2f rectPosition = p_rect.GetPosition({0,0});
	Vector2i rectDimension = p_rect.GetDimension();

	float testX = circlePosition.GetX();
	float testY = circlePosition.GetY();

	if(circlePosition.GetX() < rectPosition.GetX())
	{	
		testX = rectPosition.GetX();
	}
	else if (circlePosition.GetX() > rectPosition.GetX() + rectDimension.GetX())
	{
		testX = rectPosition.GetX() + rectDimension.GetX();
	}

	if(circlePosition.GetY() < rectPosition.GetY())
	{
		testY = rectPosition.GetY();
	}
	else if (circlePosition.GetY() > rectPosition.GetY() + rectDimension.GetY())
	{
		testY = rectPosition.GetY() + rectDimension.GetY();
	}

	Vector2f distance = {circlePosition.GetX() - testX, circlePosition.GetY() - testY};

	float sqrtLength = (distance.GetX() * distance.GetX()) + (distance.GetY() * distance.GetY());

	return sqrtLength <= (circleRadius * circleRadius);
}

bool Collision::Collision_OBB(const Collider& p_rectA, const Collider& p_rectB)
{
    auto cornersA = Utils::GetCorners(
		p_rectA.GetPosition(), p_rectA.GetDimension(),
		p_rectA.GetAngle(), p_rectA.GetRotationPoint());
    auto cornersB = Utils::GetCorners(
		p_rectB.GetPosition(), p_rectB.GetDimension(), 
		p_rectB.GetAngle(), p_rectB.GetRotationPoint());

    // 4 axes à tester : 2 par rect (les normales de leurs côtés)
    std::array<Vector2f, 4> axes;

    // Axes de A
    axes[0] = { cornersA[1].GetX() - cornersA[0].GetX(), cornersA[1].GetY() - cornersA[0].GetY() };
    axes[1] = { cornersA[3].GetX() - cornersA[0].GetX(), cornersA[3].GetY() - cornersA[0].GetY() };
    // Axes de B
    axes[2] = { cornersB[1].GetX() - cornersB[0].GetX(), cornersB[1].GetY() - cornersB[0].GetY() };
    axes[3] = { cornersB[3].GetX() - cornersB[0].GetX(), cornersB[3].GetY() - cornersB[0].GetY() };

    for (const auto& axis : axes)
    {
        auto [minA, maxA] = Project(cornersA, axis);
        auto [minB, maxB] = Project(cornersB, axis);

        if (maxA < minB || maxB < minA)
            return false;
    }

    return true; // Aucun axe séparateur = collision
}

bool Collision::Collision_OBC(const Collider& p_rect, const Collider& p_circle)
{
	//rectangle 
	Vector2i rectDimension = p_rect.GetDimension();
	Vector2f rectPosition = { p_rect.GetPosition() };
	Vector2f rectCenter = { rectPosition.GetX(), rectPosition.GetY()};
	
	float angle = p_rect.GetAngle();

	//circle
	Vector2f circleCenter = { p_circle.GetPosition().GetX(), p_circle.GetPosition().GetY()};
	float circleRadius = p_circle.GetDimension().GetX() / 2;

	//vecteur center circle-rectangle
	Vector2f vector = { circleCenter - rectCenter };

	float newAngle = Utils::DegToRad(angle);

	float x = vector.GetX() * Utils::Cos(newAngle) + vector.GetY() * Utils::Sin(newAngle);
	float y = -vector.GetX() * Utils::Sin(newAngle) + vector.GetY() * Utils::Cos(newAngle);

	Vector2f newCircleCenter = { x,y };

	float testX = newCircleCenter.GetX();
	float testY = newCircleCenter.GetY();

	if (newCircleCenter.GetX() < -rectDimension.GetX() / 2)
	{
		testX = -rectDimension.GetX() / 2;
	}
	else if (newCircleCenter.GetX() >  rectDimension.GetX() / 2)
	{
		testX = rectDimension.GetX() / 2 ;
	}

	if (newCircleCenter.GetY() < -rectDimension.GetY() / 2)
	{
		testY = -rectDimension.GetY() / 2;
	}
	else if (newCircleCenter.GetY() > rectDimension.GetY() / 2)
	{
		testY = rectDimension.GetY() /  2 ;
	}

	Vector2f distance = { newCircleCenter.GetX() - testX, newCircleCenter.GetY() - testY };

	float sqrtLength = (distance.GetX() * distance.GetX()) + (distance.GetY() * distance.GetY());

	return sqrtLength <= (circleRadius * circleRadius);
}

void Collision::ComputeManifold_AABB(const Collider& p_rectA, const Collider& p_rectB, CollisionManifold& p_manifold)
{
	Vector2f posA = p_rectA.GetPosition({ 0.f, 0.f });
	Vector2i dimA = p_rectA.GetDimension();
	Vector2f posB = p_rectB.GetPosition({ 0.f, 0.f });
	Vector2i dimB = p_rectB.GetDimension();

	Vector2f centerA = { posA.GetX() + dimA.GetX() * 0.5f, posA.GetY() + dimA.GetY() * 0.5f };
	Vector2f centerB = { posB.GetX() + dimB.GetX() * 0.5f, posB.GetY() + dimB.GetY() * 0.5f };

	float dx = centerB.GetX() - centerA.GetX();
	float dy = centerB.GetY() - centerA.GetY();

	float overlapX = (dimA.GetX() * 0.5f + dimB.GetX() * 0.5f) - Utils::AbsValue(dx);
	float overlapY = (dimA.GetY() * 0.5f + dimB.GetY() * 0.5f) - Utils::AbsValue(dy);

	// L'axe de pénétration minimale donne la normale et les sides
	if (overlapX < overlapY)
	{
		p_manifold.penetration = overlapX;
		if (dx > 0.f)
		{
			p_manifold.normal = { 1.f, 0.f };
			p_manifold.sideA = CollisionSide::LEFT;
			p_manifold.sideB = CollisionSide::RIGHT;
		}
		else
		{
			p_manifold.normal = { -1.f, 0.f };
			p_manifold.sideA = CollisionSide::RIGHT;
			p_manifold.sideB = CollisionSide::LEFT;
		}
	}
	else
	{
		p_manifold.penetration = overlapY;
		if (dy > 0.f)
		{
			p_manifold.normal = { 0.f, 1.f };
			p_manifold.sideA = CollisionSide::TOP;
			p_manifold.sideB = CollisionSide::BOTTOM;
		}
		else
		{
			p_manifold.normal = { 0.f, -1.f };
			p_manifold.sideA = CollisionSide::BOTTOM;
			p_manifold.sideB = CollisionSide::TOP;
		}
	}

	p_manifold.contactPoint = {
		(centerA.GetX() + centerB.GetX()) * 0.5f,
		(centerA.GetY() + centerB.GetY()) * 0.5f
	};
}

void Collision::ComputeManifold_CBC(const Collider& p_circleA, const Collider& p_circleB, CollisionManifold& p_manifold)
{
	Vector2f centerA = p_circleA.GetPosition();
	Vector2f centerB = p_circleB.GetPosition();

	float radiusA = p_circleA.GetDimension().GetX() / 2.f;
	float radiusB = p_circleB.GetDimension().GetX() / 2.f;

	float distance = Utils::DistanceSquared(centerA, centerB);
	float sumRadii = radiusA + radiusB;

	p_manifold.penetration = sumRadii - distance;

	if (distance > 0.0001f)
	{
		p_manifold.normal = {
			(centerA.GetX() - centerB.GetX()) / distance,
			(centerA.GetY() - centerB.GetY()) / distance
		};
	}
	else
	{
		p_manifold.normal = { 0.f, -1.f };
	}

	p_manifold.contactPoint = {
		centerB.GetX() + p_manifold.normal.GetX() * radiusB,
		centerB.GetY() + p_manifold.normal.GetY() * radiusB
	};

	p_manifold.sideA = CollisionSide::NONE;
	p_manifold.sideB = CollisionSide::NONE;
}

void Collision::ComputeManifold_ARC(const Collider& p_rect, const Collider& p_circle, CollisionManifold& p_manifold)
{
	Vector2f circleCenter = p_circle.GetPosition();
	float circleRadius = p_circle.GetDimension().GetX() / 2.f;

	Vector2f rectPos = p_rect.GetPosition({ 0.f, 0.f });
	Vector2i rectDim = p_rect.GetDimension();

	float closestX = Utils::Clamp(circleCenter.GetX(), rectPos.GetX(), rectPos.GetX() + rectDim.GetX());
	float closestY = Utils::Clamp(circleCenter.GetY(), rectPos.GetY(), rectPos.GetY() + rectDim.GetY());

	float dx = circleCenter.GetX() - closestX;
	float dy = circleCenter.GetY() - closestY;
	float distance = std::sqrt(dx * dx + dy * dy);

	p_manifold.penetration = circleRadius - distance;

	if (distance > 0.0001f)
	{
		p_manifold.normal = { dx / distance, dy / distance };
	}
	else
	{
		p_manifold.normal = { 0.f, -1.f };
	}

	p_manifold.contactPoint = { closestX, closestY };
	p_manifold.sideA = CollisionSide::NONE;
	p_manifold.sideB = CollisionSide::NONE;
}

void Collision::ComputeManifold_OBB(const Collider& p_rectA, const Collider& p_rectB, CollisionManifold& p_manifold)
{
	auto cornersA = Utils::GetCorners(p_rectA.GetPosition(), p_rectA.GetDimension(), p_rectA.GetAngle(), p_rectA.GetRotationPoint());
	auto cornersB = Utils::GetCorners(p_rectB.GetPosition(), p_rectB.GetDimension(), p_rectB.GetAngle(), p_rectB.GetRotationPoint());

	std::array<Vector2f, 4> axes;
	axes[0] = { cornersA[1].GetX() - cornersA[0].GetX(), cornersA[1].GetY() - cornersA[0].GetY() };
	axes[1] = { cornersA[3].GetX() - cornersA[0].GetX(), cornersA[3].GetY() - cornersA[0].GetY() };
	axes[2] = { cornersB[1].GetX() - cornersB[0].GetX(), cornersB[1].GetY() - cornersB[0].GetY() };
	axes[3] = { cornersB[3].GetX() - cornersB[0].GetX(), cornersB[3].GetY() - cornersB[0].GetY() };

	float smallestOverlap = FLT_MAX;
	Vector2f bestAxis;

	for (const auto& axis : axes)
	{
		Vector2f axisNorm = Utils::NormalizeVector(axis);

		auto [minA, maxA] = Project(cornersA, axisNorm);
		auto [minB, maxB] = Project(cornersB, axisNorm);

		float overlap = Utils::Min({ maxA, maxB }) - Utils::Max({ minA, minB });

		if (overlap < smallestOverlap && overlap > 0.f)
		{
			smallestOverlap = overlap;
			bestAxis = axisNorm;
		}
	}

	Vector2f direction = p_rectA.GetPosition() - p_rectB.GetPosition();
	if (Utils::Dot(direction, bestAxis) < 0.f)
		bestAxis = { -bestAxis.GetX(), -bestAxis.GetY() };

	p_manifold.penetration = smallestOverlap;
	p_manifold.normal = bestAxis;
	p_manifold.contactPoint = {
		(p_rectA.GetPosition().GetX() + p_rectB.GetPosition().GetX()) * 0.5f,
		(p_rectA.GetPosition().GetY() + p_rectB.GetPosition().GetY()) * 0.5f
	};
	p_manifold.sideA = CollisionSide::NONE;
	p_manifold.sideB = CollisionSide::NONE;
}

void Collision::ComputeManifold_OBC(const Collider& p_rect, const Collider& p_circle, CollisionManifold& p_manifold)
{
	Vector2i rectDimension = p_rect.GetDimension();
	Vector2f rectCenter = p_rect.GetPosition();
	float angle = p_rect.GetAngle();
	float radAngle = Utils::DegToRad(angle);

	Vector2f circleCenter = p_circle.GetPosition();
	float circleRadius = p_circle.GetDimension().GetX() / 2.f;

	// Transformer le centre du cercle dans le repère local du rectangle 
	Vector2f vector = { circleCenter.GetX() - rectCenter.GetX(), circleCenter.GetY() - rectCenter.GetY() };

	float localX = vector.GetX() * Utils::Cos(radAngle) + vector.GetY() * Utils::Sin(radAngle);
	float localY = -vector.GetX() * Utils::Sin(radAngle) + vector.GetY() * Utils::Cos(radAngle);

	Vector2f localCircle = { localX, localY };

	// Point le plus proche dans le repère local du rectangle
	float closestX = Utils::Clamp(localCircle.GetX(), -rectDimension.GetX() / 2.f, rectDimension.GetX() / 2.f);
	float closestY = Utils::Clamp(localCircle.GetY(), -rectDimension.GetY() / 2.f, rectDimension.GetY() / 2.f);

	float dx = localCircle.GetX() - closestX;
	float dy = localCircle.GetY() - closestY;
	float distance = std::sqrt(dx * dx + dy * dy);

	p_manifold.penetration = circleRadius - distance;

	// Normale dans le repère local puis retransformée en repère monde
	Vector2f localNormal;
	if (distance > 0.0001f)
		localNormal = { dx / distance, dy / distance };
	else
		localNormal = { 0.f, -1.f };

	float worldNX = localNormal.GetX() * Utils::Cos(radAngle) - localNormal.GetY() * Utils::Sin(radAngle);
	float worldNY = localNormal.GetX() * Utils::Sin(radAngle) + localNormal.GetY() * Utils::Cos(radAngle);

	p_manifold.normal = { worldNX, worldNY };

	// Point de contact en repère monde
	float worldClosestX = closestX * Utils::Cos(radAngle) - closestY * Utils::Sin(radAngle) + rectCenter.GetX();
	float worldClosestY = closestX * Utils::Sin(radAngle) + closestY * Utils::Cos(radAngle) + rectCenter.GetY();

	p_manifold.contactPoint = { worldClosestX, worldClosestY };
	p_manifold.sideA = CollisionSide::NONE;
	p_manifold.sideB = CollisionSide::NONE;
}

bool Collision::IsColliding(const Collider& p_colliderA, const Collider& p_colliderB)
{
	Shape shapeA = p_colliderA.GetShape();
	Shape shapeB = p_colliderB.GetShape();

	if (shapeA == Shape::Rectangle && shapeB == Shape::Rectangle)
	{
		if (p_colliderA.GetAngle() != 0 || p_colliderB.GetAngle() != 0)
			return Collision_OBB(p_colliderA, p_colliderB);
		else
			return Collision_AABB(p_colliderA, p_colliderB);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Circle)
	{
		return Collision_CBC(p_colliderA, p_colliderB);
	}
	else if (shapeA == Shape::Rectangle && shapeB == Shape::Circle)
	{
		if (p_colliderA.GetAngle() != 0 || p_colliderB.GetAngle() != 0)
			return Collision_OBC(p_colliderA, p_colliderB);
		else
			return Collision_ARC(p_colliderA, p_colliderB);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Rectangle)
	{
		return Collision_ARC(p_colliderB, p_colliderA);
	}

	return false;
}

CollisionManifold Collision::ComputeManifold(const Collider& p_colliderA, const Collider& p_colliderB)
{
	CollisionManifold manifold;

	if (IsColliding(p_colliderA, p_colliderB) == false)
		return manifold;

	Shape shapeA = p_colliderA.GetShape();
	Shape shapeB = p_colliderB.GetShape();
	bool rotated = (p_colliderA.GetAngle() != 0.f || p_colliderB.GetAngle() != 0.f);

	if (shapeA == Shape::Rectangle && shapeB == Shape::Rectangle)
	{
		if (rotated)
			ComputeManifold_OBB(p_colliderA, p_colliderB, manifold);
		else
			ComputeManifold_AABB(p_colliderA, p_colliderB, manifold);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Circle)
	{
		ComputeManifold_CBC(p_colliderA, p_colliderB, manifold);
	}
	else if (shapeA == Shape::Rectangle && shapeB == Shape::Circle)
	{
		if (rotated)
			ComputeManifold_OBC(p_colliderA, p_colliderB, manifold);
		else
			ComputeManifold_ARC(p_colliderA, p_colliderB, manifold);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Rectangle)
	{
		if (rotated)
			ComputeManifold_OBC(p_colliderB, p_colliderA, manifold);
		else
			ComputeManifold_ARC(p_colliderB, p_colliderA, manifold);

		manifold.normal = { -manifold.normal.GetX(), -manifold.normal.GetY() };
		CollisionSide tmp = manifold.sideA;
		manifold.sideA = manifold.sideB;
		manifold.sideB = tmp;
	}

	manifold.valid = true;
	return manifold;
}