#pragma once

#include "network-entity.h"

constexpr float DEFAULT_PROJECTILE_LIFETIME = 2.0f;
constexpr float DEFAULT_PROJECTILE_SPEED = 20.0f;

class ProjectileEntity : public NetworkEntity {
public:
    ProjectileEntity(OwnerID _owner, Vector3 _position, XMFLOAT4 _rotation);

protected:
    void OnUpdate() override;
    
private:
};