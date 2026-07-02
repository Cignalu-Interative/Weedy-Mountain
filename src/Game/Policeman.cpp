#include "pch.h"
#include "Policeman.h"

void Policeman::OnInitialize()
{
	AddCollider("detection", { 80, 80 }, Shape::Circle, { 0.f, 0.f }, true);
}

void Policeman::OnUpdate()
{
}

void Policeman::OnEvent()
{
}

void Policeman::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
}

void Policeman::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
}


