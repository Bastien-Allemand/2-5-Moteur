#pragma once

#include "server-entity.h"

class ServerCoinEntity : public ServerEntity
{
public:
    ServerCoinEntity(cpu_transform _transform, cpu_aabb _hitbox);

    void OnUpdate() override;
    
    void MoveCoin();
protected:

private:
};
