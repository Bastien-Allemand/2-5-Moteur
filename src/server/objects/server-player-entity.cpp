#include "pch.h"
#include "server-player-entity.h"

#include "../services/server-service.h"

#include "../messages/move-message.h"
#include "../messages/fire-message.h"
#include "../messages/look-message.h"

#include <framework/application.h>

#include "server-projectile-entity.h"
#include "messages/upstatus-message.h"
#include "services/game-service.h"

// From stackoverflow : https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
int random(int min, int max) //range : [min, max]
{
	static bool first = true;
	if (first) 
	{  
		srand( time(NULL) ); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (( max + 1 ) - min);
}

ServerPlayerEntity::ServerPlayerEntity(cpu_transform _transform, cpu_aabb _hitbox, OwnerID _owner, uint _maxHealth)
	: ServerNetworkEntity(_transform, _hitbox, _owner) {
	m_maxHealth = _maxHealth;
	m_currentHealth = _maxHealth;
	
	m_fireTimer = 1.0f;
	m_fireTime = 0.0f;
	
	m_spawnTimer = 3.0f;
	m_spawnTime = 0.0f;
	m_state = PlayerState::Alive;

	ServerService& server = APPLICATION.getService<ServerService>();
	server.AddMessageListener<MoveMessage>(this, &ServerPlayerEntity::OnMoveRequested);
	server.AddMessageListener<LookMessage>(this, &ServerPlayerEntity::OnLookRequested);
	server.AddMessageListener<FireMessage>(this, &ServerPlayerEntity::OnFireRequested);
	
	TeleportToRandomLocation();
}

void ServerPlayerEntity::OnMoveRequested(MoveMessage* _message, OwnerID _sender) {
	if (_sender != getOwner())
		return;

	m_lastInput.dirX = _message->dirX;
	m_lastInput.dirY = _message->dirY;
	m_lastInput.dirZ = _message->dirZ;
}

void ServerPlayerEntity::OnLookRequested(LookMessage* _message, OwnerID _sender) {
	if (_sender != getOwner())
		return;
	
	float x = _message->posX;
	float y = _message->posY;
	float z = _message->posZ;
	
	m_transform.LookAt(x, z, y);
}

void ServerPlayerEntity::OnFireRequested(FireMessage* _message, OwnerID _sender) {
	if (_sender != getOwner())
		return;
	
	if (!m_canFire)
		return;
	
	// SHOOT !
	FireMessage fire;
	fire.posX = m_transform.pos.x + m_transform.dir.x;
	fire.posY = m_transform.pos.y + m_transform.dir.y;
	fire.posZ = m_transform.pos.z + m_transform.dir.z;
	
	fire.rotX = m_transform.quat.x;
	fire.rotY = m_transform.quat.y;
	fire.rotZ = m_transform.quat.z;
	fire.rotW = m_transform.quat.w;
	
	fire.owner = getOwner();
	
	// TEMP SERVER SPAWNING
	GameService& game = APPLICATION.getService<GameService>();
	cpu_transform transform;
	transform.SetPosition(m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);
	transform.quat = m_transform.quat;
	transform.SetRotationFromQuaternion();
	cpu_aabb aabb;
	aabb.min.x = -1; aabb.min.y = -1; aabb.min.z = -1;
	aabb.max.x = 1; aabb.max.y = 1; aabb.max.z = 1;
	game.spawn<ServerProjectileEntity>(transform, aabb, getOwner());
	
	ServerService& server = APPLICATION.getService<ServerService>();
	server.BroadcastMessage(fire);
	
	m_fireMutex.lock();
	m_canFire = false;
	m_fireTime = 0.0f;
	m_fireMutex.unlock();
}

void ServerPlayerEntity::Move() {
	float x = m_transform.pos.x + m_lastInput.dirX * DEFAULT_PLAYER_SPEED * cpuTime.delta;
	float y = m_transform.pos.y + m_lastInput.dirY * DEFAULT_PLAYER_SPEED * cpuTime.delta;
	float z = m_transform.pos.z + m_lastInput.dirZ * DEFAULT_PLAYER_SPEED * cpuTime.delta;
	
	x = std::clamp(x, -15.0f, 15.0f);
	z = std::clamp(z, -8.0f, 8.0f);
	
	m_transform.SetPosition(x, y, z);
	
	MoveMessage move;
	move.posX = m_transform.pos.x;
	move.posY = m_transform.pos.y;
	move.posZ = m_transform.pos.z;
	move.owner = getOwner();
	
	ServerService& server = APPLICATION.getService<ServerService>();
	server.BroadcastMessage(move);
}

void ServerPlayerEntity::Look() {
	LookMessage look;
	look.rotX = m_transform.quat.x;
	look.rotY = m_transform.quat.y;
	look.rotZ = m_transform.quat.z;
	look.rotW = m_transform.quat.w;
	look.owner = getOwner();
	
	ServerService& server = APPLICATION.getService<ServerService>();
	server.BroadcastMessage(look);
}
void ServerPlayerEntity::Die() {
	ServerService& server = APPLICATION.getService<ServerService>();
	Owner* owner = server.getOwner(getOwner());
	int* ptr = (int*)owner->userData;
		
	UpStatus_Message status;
	status.health = 0;
	status.score = *ptr;
	status.ownerid = getOwner();
	status.username = owner->username;
	server.BroadcastMessage(status);
	
	m_state = PlayerState::Dead;
}
void ServerPlayerEntity::TeleportToRandomLocation() {
	GameService& game = APPLICATION.getService<GameService>();
	
	float randX = static_cast<float>(random(-14, 14));
	float Y = 0.0f;
	float randZ = static_cast<float>(random(-7, 7));
	
	bool valid = false;
	while (!valid) {
		for (ServerEntity* entity : game.getEntities()) {
			float dx = randX - m_transform.pos.x;
			float dy = Y - m_transform.pos.y;
			float dz = randZ - m_transform.pos.z;
		
			float distanceSq = dx * dx + dy * dy + dz * dz;
			float radiusSum = 2.5;
		
			if (distanceSq < radiusSum * radiusSum) {
				break;
			}
		}
		valid = true;
	}
	
	m_transform.SetPosition(randX, Y, randZ);
}

void ServerPlayerEntity::OnSpawned() {
	ServerEntity::OnSpawned();
	
	ServerService& server = APPLICATION.getService<ServerService>();
	Owner* owner = server.getOwner(getOwner());
	int* ptr = (int*)owner->userData;
	(*ptr)=0;
	
	UpStatus_Message status;
	status.health = m_currentHealth;
	status.score = 0;
	status.ownerid = getOwner();
	status.username = owner->username;
	server.BroadcastMessage<UpStatus_Message>(status);
}

void ServerPlayerEntity::OnUpdate() {
	ServerEntity::OnUpdate();
	
	if (m_state == PlayerState::Alive) {
		Move();
		Look();
	}


	m_fireMutex.lock();
	if (m_fireTime < m_fireTimer) {
		m_fireTime += cpuTime.delta;
	}
	else {
		m_canFire = true;
	}
	m_fireMutex.unlock();
	
	if (m_state == PlayerState::Dead) {
		if (m_spawnTime < m_spawnTimer) {
			m_spawnTime += cpuTime.delta;
		}
		else {
			// SPAWN
			m_currentHealth = m_maxHealth;
			TeleportToRandomLocation();
	
			ServerService& server = APPLICATION.getService<ServerService>();
			Owner* owner = server.getOwner(getOwner());
			int* ptr = (int*)owner->userData;
		
			UpStatus_Message status;
			status.health = m_currentHealth;
			status.score = *ptr;
			status.ownerid = getOwner();
			status.username = owner->username;
			server.BroadcastMessage(status);
			m_state = PlayerState::Alive;
			m_spawnTime = 0;
		}
	}
}

void ServerPlayerEntity::OnDestroy() {
	ServerEntity::OnDestroy();
}

bool ServerPlayerEntity::takeDamage(uint _amount) {
	if (m_currentHealth - _amount <= 0) {
		m_currentHealth = 0;
		Die();
		return true;
	}
	else {
		m_currentHealth -= _amount;
		
		ServerService& server = APPLICATION.getService<ServerService>();
		Owner* owner = server.getOwner(getOwner());
		int* ptr = (int*)owner->userData;
		
		UpStatus_Message status;
		status.health = m_currentHealth;
		status.score = *ptr;
		status.ownerid = getOwner();
		status.username = owner->username;
		server.BroadcastMessage(status);
	}
	
	return false;
}

void ServerPlayerEntity::heal(uint _amount) {
	m_currentHealth += _amount;
	if (m_currentHealth > m_maxHealth)
		m_currentHealth = m_maxHealth;
}

uint ServerPlayerEntity::getHealth() const {
	return m_currentHealth;
}