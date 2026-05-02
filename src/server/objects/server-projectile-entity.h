#pragma once

#include "server-network-entity.h"

constexpr float DEFAULT_PROJECTILE_LIFETIME = 2.0f;
constexpr float DEFAULT_PROJECTILE_SPEED = 20.0f;
constexpr uint DEFAULT_PROJECTILE_DAMAGE = 25;

class ServerProjectileEntity : public ServerNetworkEntity {
public:
    ServerProjectileEntity(cpu_transform _transform, cpu_aabb _hitbox, OwnerID _owner);
protected:
    void OnUpdate() override;
    void OnCollision(ServerEntity* _other) override;

private:
    float m_lifetime = DEFAULT_PROJECTILE_LIFETIME;
};