#include "pch.h"
#include "projectile-entity.h"

ProjectileEntity::ProjectileEntity(OwnerID _owner, Vector3 _position, XMFLOAT4 _rotation)
    : NetworkEntity(_owner, ModelType::SPHERE, _position) {
    m_entity->transform.quat = _rotation;
    m_entity->transform.SetRotationFromQuaternion();
}

void ProjectileEntity::OnUpdate() {
    NetworkEntity::OnUpdate();
    
    float x = m_entity->transform.pos.x + (m_entity->transform.dir.x * cpuTime.delta * DEFAULT_PROJECTILE_SPEED);
    float y = m_entity->transform.pos.y + (m_entity->transform.dir.y * cpuTime.delta * DEFAULT_PROJECTILE_SPEED);
    float z = m_entity->transform.pos.z + (m_entity->transform.dir.z * cpuTime.delta * DEFAULT_PROJECTILE_SPEED);
    
    m_entity->transform.SetPosition(x, y, z);
    
    if (m_entity->lifetime > DEFAULT_PROJECTILE_LIFETIME) {
        destroy();
    }
}