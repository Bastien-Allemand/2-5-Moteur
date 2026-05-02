#include "pch.h"
#include "server-entity.h"

#include "framework/application.h"
#include "services/game-service.h"

ServerEntity::ServerEntity(cpu_transform _transform, cpu_aabb _hitbox) {
	m_transform = _transform;
	m_collider = _hitbox;
}

void ServerEntity::destroy() {
	GameService& game = APPLICATION.getService<GameService>();
	game.destroy(this);
}

void ServerEntity::OnSpawned() {
}

void ServerEntity::OnUpdate() {
	m_transform.UpdateWorld();
	m_collider.Transform(XMLoadFloat4x4(&m_transform.GetWorld()));
}

void ServerEntity::OnDestroy() {
}

void ServerEntity::OnCollision(ServerEntity* _other) {
}