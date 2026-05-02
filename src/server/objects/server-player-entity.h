#pragma once

#include "server-network-entity.h"
#include "utils/threading/mutex.h"

struct FireMessage;
struct MoveMessage;
struct LookMessage;

constexpr float DEFAULT_PLAYER_SPEED = 8.0f;

struct PlayerInput {
	float dirX = 0.f;
	float dirY = 0.f;
	float dirZ = 0.f;
};

enum class PlayerState {
	Alive,
	Dead
};

class ServerPlayerEntity : public ServerNetworkEntity {
public:
	ServerPlayerEntity(cpu_transform _transform, cpu_aabb _hitbox, OwnerID _owner, uint _maxHealth = 100);

	void OnSpawned() override;
	void OnUpdate() override;
	void OnDestroy() override;

	/// Returns 'true' if the player died
	bool takeDamage(uint _amount);
	void heal(uint _amount);

	uint getHealth() const;

protected:
	void OnMoveRequested(MoveMessage* _message, OwnerID _sender);
	void OnLookRequested(LookMessage* _message, OwnerID _sender);
	void OnFireRequested(FireMessage* _message, OwnerID _sender);
	
	void Move();
	void Look();
	void Die();
	
	void TeleportToRandomLocation();
	
private:
	PlayerInput m_lastInput;
	PlayerState m_state;
	
	uint m_currentHealth;
	uint m_maxHealth;
	
	float m_fireTimer;
	float m_fireTime;
	
	float m_spawnTimer;
	float m_spawnTime;
	
	bool m_canFire;
	Mutex m_fireMutex;
	
};