#pragma once
#include "entity.h"
#include "framework/networking/owner.h"

struct UpCoin_Message;

class CoinEntity : public Entity
{
public:
    CoinEntity(Vector3 _position = Vector3::zero());
    void Move(Vector3 _position);
protected:
    void OnSpawned() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void OnRender(int _pass) override;
    
    void OnMove(UpCoin_Message* _message, OwnerID _owner);
private:

};
