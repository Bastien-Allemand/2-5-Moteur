#include "pch.h"
#include "server-network-entity.h"

ServerNetworkEntity::ServerNetworkEntity(cpu_transform _transform, cpu_aabb _hitbox, OwnerID _owner)
	: ServerEntity(_transform, _hitbox) {
	m_owner = _owner;
}

bool ServerNetworkEntity::isOwnedBy(OwnerID _owner) const {
	return m_owner == _owner;
}

OwnerID ServerNetworkEntity::getOwner() const {
	return m_owner;
}