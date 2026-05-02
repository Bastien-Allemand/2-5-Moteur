#pragma once

#include "server-entity.h"

#include <framework/networking/owner.h>

class ServerNetworkEntity : public ServerEntity {
public:
	ServerNetworkEntity(cpu_transform _transform, cpu_aabb _hitbox, OwnerID _owner);

	bool isOwnedBy(OwnerID _owner) const;

	OwnerID getOwner() const;

protected:
	OwnerID m_owner;
};