#pragma once

#include <common/framework/service.h>

#include <framework/networking/owner.h>

#include "messages/coin-message.h"
#include "objects/entity.h"

struct FireMessage;
struct PlayerJoinedMessage;

class RenderService : public Service {
public:
	RenderService();
	~RenderService() override;
	
	template <typename T, typename... Args>
	T& spawn(Args... _args);
	
	void destroy(Entity* _entity);
	
protected:
	void OnStartup() override;
	void OnUpdate() override;
	void OnShutdown() override;
	
	void OnRender(int _pass);
	
	void OnPlayerJoined(PlayerJoinedMessage* _message, OwnerID _owner);
	void OnFireMissile(FireMessage* _message, OwnerID _owner);
	void OnCoinSpawned(CoinSpawnMessage* _message, OwnerID _owner);
private:
	cpu_font m_font;
	cpu_engine m_engine;
	
	std::vector<Entity*> m_toAdd;
	std::vector<Entity*> m_toRemove;
	
	std::vector<Entity*> m_entities;
};
#include "render-service.inl"