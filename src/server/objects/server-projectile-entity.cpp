#include "pch.h"
#include "server-projectile-entity.h"

#include <iostream>

#include "server-coin-entity.h"
#include "server-player-entity.h"
#include "framework/application.h"
#include "messages/upcoin-message.h"
#include "messages/upstatus-message.h"
#include "services/server-service.h"

ServerProjectileEntity::ServerProjectileEntity(cpu_transform _transform, cpu_aabb _hitbox, OwnerID _owner) :
    ServerNetworkEntity(_transform, _hitbox, _owner) {
}

void ServerProjectileEntity::OnUpdate() {
    ServerNetworkEntity::OnUpdate();
    
    float x = m_transform.pos.x + (m_transform.dir.x * cpuTime.delta * DEFAULT_PROJECTILE_SPEED);
    float y = m_transform.pos.y + (m_transform.dir.y * cpuTime.delta * DEFAULT_PROJECTILE_SPEED);
    float z = m_transform.pos.z + (m_transform.dir.z * cpuTime.delta * DEFAULT_PROJECTILE_SPEED);
    
    m_transform.SetPosition(x, y, z);
    
    float dt = cpuTime.delta;
    m_lifetime -= dt;
    
    if (m_lifetime < 0) {
        destroy();
    }
}

void ServerProjectileEntity::OnCollision(ServerEntity* _other) {
    ServerNetworkEntity::OnCollision(_other);
    
    if (ServerCoinEntity* coin = dynamic_cast<ServerCoinEntity*>(_other)) {
        ServerService& serv = APPLICATION.getService<ServerService>();
        Owner* owner = serv.getOwner(getOwner());
        int* ptr = (int*)owner->userData;
        (*ptr) += 1;
        coin->MoveCoin();
        
        UpStatus_Message status;
        status.health = 100; // TEMP : TO DO REMOVE !!
        status.score = *ptr;
        status.ownerid = getOwner();
		status.username = owner->username;
        serv.BroadcastMessage(status);
    }
    
    if (ServerPlayerEntity* player = dynamic_cast<ServerPlayerEntity*>(_other)) {
        if (player->getOwner() == getOwner())
            return;
        bool died = player->takeDamage(DEFAULT_PROJECTILE_DAMAGE);
        if (died) {
            OwnerID ownerID = getOwner();
            
            ServerService& server = APPLICATION.getService<ServerService>();
            Owner* owner = server.getOwner(ownerID);
            int* ptr = (int*)owner->userData;
            (*ptr)++;
		
            UpStatus_Message status;
            status.health = 100; // TEMP : TO DO REMOVE !!
            status.score = *ptr;
            status.ownerid = getOwner();
			status.username = owner->username;
            server.BroadcastMessage(status);
        }
        
        destroy();
    }
}