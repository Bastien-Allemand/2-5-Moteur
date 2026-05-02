#pragma once

#include "objects/server-entity.h"

#include <framework/service.h>
#include <vector>

#include "framework/networking/owner.h"

class HandshakeMessage;

class GameService : public Service {
public:
	GameService();
	~GameService() override = default;
	
	template <typename T, typename... Args>
	T& spawn(Args... _args);
	
	void destroy(ServerEntity* _entity);
	
	const std::vector<ServerEntity*>& getEntities() const;

protected:
	void OnStartup() override;
	void OnUpdate() override;
	void OnShutdown() override;
	
	// Listeners
	void OnPlayerConnected(OwnerID _owner);

private:
	std::vector<ServerEntity*> m_toAdd;
	std::vector<ServerEntity*> m_toRemove;
	
	std::vector<ServerEntity*> m_entities;
};
#include "game-service.inl"