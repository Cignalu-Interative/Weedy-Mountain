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

void Collision::Repulse_AABB(Entity* p_entityRectA, Entity* p_entityRectB)
{
	Collider* colliderA = p_entityRectA->GetCollider();
	Collider* colliderB = p_entityRectB->GetCollider();

	Vector2f centerA = colliderA->GetPosition();
	Vector2f centerB = colliderB->GetPosition();

	Vector2i dimensionA = colliderA->GetDimension();
	Vector2i dimensionB = colliderB->GetDimension();

	float dx = centerA.GetX() - centerB.GetX();  // Positif = A à droite de B
	float dy = centerA.GetY() - centerB.GetY();  // Positif = A au-dessus de B

	// Calcul de l'overlap sur les axes x et y
	// L'overlap est la distance de pénétration entre les deux rectangles sur chaque axe
	float overlapX = (dimensionA.GetX() * 0.5f + dimensionB.GetX() * 0.5f) - Utils::AbsValue(dx);
	float overlapY = (dimensionA.GetY() * 0.5f + dimensionB.GetY() * 0.5f) - Utils::AbsValue(dy);

	float pushX = overlapX * (dx < 0 ? -1.f : 1.f);
	float pushY = overlapY * (dy < 0 ? -1.f : 1.f);

	/*float massA = p_entityRectA->GetPhysicsBody() ? p_entityRectA->GetPhysicsBody()->GetMass() : 1.f;
	float massB = p_entityRectB->GetPhysicsBody() ? p_entityRectB->GetPhysicsBody()->GetMass() : 1.f;
	float totalMass = massA + massB;*/

	float invMassA = p_entityRectA->GetPhysicsBody() ? p_entityRectA->GetPhysicsBody()->GetInvMass() : 1.f;
	float invMassB = p_entityRectB->GetPhysicsBody() ? p_entityRectB->GetPhysicsBody()->GetInvMass() : 1.f;
	float totalInvMass = invMassA + invMassB;

	if (totalInvMass == 0.0f) return; // deux Static donc rien à faire

	/*float ratioA = massB / totalMass;
	float ratioB = massA / totalMass;*/

	float ratioA = invMassA / totalInvMass; // Dynamic=1 -> ratio=1, Static=0 -> ratio=0
	float ratioB = invMassB / totalInvMass;

	if (overlapX < overlapY)
	{
		p_entityRectA->Move({ pushX * ratioA, 0 });
		p_entityRectB->Move({ -pushX * ratioB, 0 });
	}
	else
	{
		p_entityRectA->Move({ 0, pushY * ratioA });
		p_entityRectB->Move({ 0, -pushY * ratioB });
	}
}

void Collision::Repulse_CBC(Entity* p_entityCircleA, Entity* p_entityCircleB)
{
	Collider* colliderA = p_entityCircleA->GetCollider();
	Collider* colliderB = p_entityCircleB->GetCollider();

	Vector2f centerA = colliderA->GetPosition();
	Vector2f centerB = colliderB->GetPosition();

	int radiusA = colliderA->GetDimension().GetX() / 2;
	int radiusB = colliderB->GetDimension().GetX() / 2;

	Vector2f distance = centerA - centerB;

	float lenght = Utils::DistanceSquared(centerA, centerB);

	float overlap = (lenght - (radiusA + radiusB)) * 0.5f;

	Vector2f normal = distance / lenght;

	Vector2f translation = overlap * normal;

	p_entityCircleA->Move(-translation);
	p_entityCircleB->Move(translation);
}

void Collision::Repulse_ARC(Entity* p_entityRect, Entity* p_entityCircle)
{

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

bool Collision::IsColliding(const Collider& p_colliderA, const Collider& p_colliderB)
{
	Shape shapeA = p_colliderA.GetShape();
	Shape shapeB = p_colliderB.GetShape();

	if (shapeA == Shape::Rectangle && shapeB == Shape::Rectangle)
	{
		return Collision_AABB(p_colliderA, p_colliderB);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Circle)
	{
		return Collision_CBC(p_colliderA, p_colliderB);
	}
	else if (shapeA == Shape::Rectangle && shapeB == Shape::Circle)
	{
		return Collision_ARC(p_colliderA, p_colliderB);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Rectangle)
	{
		return Collision_ARC(p_colliderB, p_colliderA);
	}

	return false;
}

void Collision::Repulse(Entity* p_entityA, Entity* p_entityB)
{
	Shape shapeA = p_entityA->GetShape();
	Shape shapeB = p_entityB->GetShape();

	if (shapeA == Shape::Rectangle && shapeB == Shape::Rectangle)
	{
		return Repulse_AABB(p_entityA, p_entityB);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Circle)
	{
		return Repulse_CBC(p_entityA, p_entityB);
	}
	else if (shapeA == Shape::Rectangle && shapeB == Shape::Circle)
	{
		return Repulse_ARC(p_entityA, p_entityB);
	}
	else if (shapeA == Shape::Circle && shapeB == Shape::Rectangle)
	{
		return Repulse_ARC(p_entityB, p_entityA);
	}
}