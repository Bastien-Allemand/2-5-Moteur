#include "pch.h"
#include "server-coin-entity.h"

#include "framework/application.h"
#include "messages/upcoin-message.h"
#include "services/server-service.h"

int random_2(int min, int max) //range : [min, max]
{
    static bool first = true;
    if (first) 
    {  
        srand( time(NULL) ); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}

ServerCoinEntity::ServerCoinEntity(cpu_transform _transform, cpu_aabb _hitbox) : ServerEntity(_transform, _hitbox) {
    

    int posX = static_cast<float>(random_2(-14, 14));
    int posZ = static_cast<float>(random_2(-7, 7));
        
    m_transform.SetPosition(posX, 0.0f, posZ);
    
    UpCoin_Message upCoin;
    upCoin.PosX = posX;
    upCoin.PosY = posZ;
    upCoin.alive = true;
    
    ServerService& server = APPLICATION.getService<ServerService>();
    server.BroadcastMessage(upCoin);
}

void ServerCoinEntity::OnUpdate() {
    ServerEntity::OnUpdate();
    
    UpCoin_Message upCoin;
    upCoin.PosX = m_transform.pos.x;
    upCoin.PosY = m_transform.pos.z;
    upCoin.alive = true;
    
    ServerService& server = APPLICATION.getService<ServerService>();
    server.BroadcastMessage(upCoin);
}

void ServerCoinEntity::MoveCoin() {
    int posX = static_cast<float>(random_2(-14, 14));
    int posZ = static_cast<float>(random_2(-7, 7));
    m_transform.SetPosition(posX, 0.0f, posZ);
    
    UpCoin_Message upCoin;
    upCoin.PosX = posX;
    upCoin.PosY = posZ;
    upCoin.alive = true;
    
    ServerService& server = APPLICATION.getService<ServerService>();
    server.BroadcastMessage(upCoin);
}
