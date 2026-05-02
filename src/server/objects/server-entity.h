#pragma once

#include <cpu-engine.h>

class ServerEntity {
public:
	virtual ~ServerEntity() = default;
	
	ServerEntity(cpu_transform _transform, cpu_aabb _hitbox);
	
	void destroy();

	virtual void OnSpawned();
	virtual void OnUpdate();
	virtual void OnDestroy();
	virtual void OnCollision(ServerEntity* _other);
protected:
	cpu_obb m_collider;
	cpu_transform m_transform;
	
	friend class GameService;
};