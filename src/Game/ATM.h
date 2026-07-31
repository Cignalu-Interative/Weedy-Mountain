#pragma once
#include "2DPendentEngine\Entity.h"

class UiATM;
class ATM :
    public Entity
{
private : 
    bool m_isInPlayerZone = false;
    UiATM* m_atmUI = nullptr;
public: 

    void OnInitialize() override; 
    void OnUpdate() override;
    
    bool IsInPlayerZone() { return m_isInPlayerZone; }

    void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side);
    void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side);

    void Toogle();
    bool IsOpen();

};

