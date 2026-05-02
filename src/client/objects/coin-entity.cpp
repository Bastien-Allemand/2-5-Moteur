#include "pch.h"
#include "coin-entity.h"

#include "framework/application.h"
#include "../messages/upcoin-message.h"
#include "services/client-service.h"

CoinEntity::CoinEntity(Vector3 _position) : Entity(ModelType::SPHERE, _position) {
    ClientService& client = APPLICATION.getService<ClientService>();
    client.AddMessageListener<UpCoin_Message>(this, &CoinEntity::OnMove);
}

void CoinEntity::Move(Vector3 _position) {
    m_entity->transform.SetPosition(_position.x, _position.y, _position.z);
}

void CoinEntity::OnSpawned() {
    Entity::OnSpawned();
}

void CoinEntity::OnUpdate() {
    Entity::OnUpdate();
}

void CoinEntity::OnDestroy() {
    Entity::OnDestroy();
}

void CoinEntity::OnRender(int _pass) {
    Entity::OnRender(_pass);
}

void CoinEntity::OnMove(UpCoin_Message* _message, OwnerID _owner) {
    Move({_message->PosX, 0.0f, _message->PosY});
}