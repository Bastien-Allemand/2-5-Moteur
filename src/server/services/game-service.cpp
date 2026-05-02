#include "pch.h"
#include "game-service.h"

#include "server-service.h"
#include "framework/application.h"
#include "objects/server-coin-entity.h"
#include "objects/server-player-entity.h"

GameService::GameService() : Service() {
	ServerService& server = APPLICATION.getService<ServerService>();
	server.OnPlayerConnected.AddListener(&GameService::OnPlayerConnected, this);
}

void GameService::destroy(ServerEntity* _entity) {
	m_toRemove.push_back(_entity);
}

const std::vector<ServerEntity*>& GameService::getEntities() const {
	return m_entities;
}

void GameService::OnStartup() {
	cpuTime.GetInstance();

	cpu_transform transform;
	transform.SetPosition(0, 0, 0);
	transform.SetYPR(0, 0, 0);
	
	cpu_aabb aabb;
	aabb.min.x = -1; aabb.min.y = -1; aabb.min.z = -1;
	aabb.max.x = 1; aabb.max.y = 1; aabb.max.z = 1;
	spawn<ServerCoinEntity>(transform, aabb);
}

void GameService::OnUpdate() {
	for (ServerEntity* entity : m_entities) {
		entity->OnUpdate();
	}
	
	// Check for collisions
	if (m_entities.size() > 1)
	{
		// Not working
		// for (int i = 0; i < m_entities.size() - 1; ++i) {
		// 	cpu_obb* first = &(m_entities[i]->m_collider);
		// 	cpu_obb* second = &(m_entities[i + 1]->m_collider);
		// 	
		// 	if (cpu::ObbObb(*first, *second)) {
		// 		std::cout << "Collision detected" << std::endl;
		// 	}
		// }
		
		for (size_t i = 0; i < m_entities.size(); ++i)
		{
			for (size_t j = i + 1; j < m_entities.size(); ++j)
			{
				float dx = m_entities[i]->m_transform.pos.x - m_entities[j]->m_transform.pos.x;
				float dy = m_entities[i]->m_transform.pos.y - m_entities[j]->m_transform.pos.y;
				float dz = m_entities[i]->m_transform.pos.z - m_entities[j]->m_transform.pos.z;

				float distanceSq = dx * dx + dy * dy + dz * dz;
				float radiusSum = 1.5f;

				if (distanceSq < radiusSum * radiusSum)
				{
					m_entities[i]->OnCollision(m_entities[j]);
					m_entities[j]->OnCollision(m_entities[i]);
				}
			}
		}
	}
	
	// Add & Remove entities at the end of the frame
	for (ServerEntity* entity : m_toAdd) {
		entity->OnSpawned();
		m_entities.push_back(entity);
	}
	m_toAdd.clear();
	
	for (ServerEntity* entity : m_toRemove) {
		entity->OnDestroy();
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
		delete entity;
	}
	m_toRemove.clear();
}

void GameService::OnShutdown() {
	for(ServerEntity* entity : m_entities) {
		delete entity;
	}
	m_entities.clear();
}
void GameService::OnPlayerConnected(OwnerID _owner) {
	ServerService& server = APPLICATION.getService<ServerService>();
	Owner* owner = server.getOwner(_owner);
	
	if (!owner) return;
	
	cpu_transform transform;
	transform.SetPosition(0, 0, 0);
	transform.SetYPR(0, 0, 0);
	
	cpu_aabb aabb;
	aabb.min.x = -1; aabb.min.y = -1; aabb.min.z = -1;
	aabb.max.x = 1; aabb.max.y = 1; aabb.max.z = 1;
	
	spawn<ServerPlayerEntity>(transform, aabb, _owner, 100);
}