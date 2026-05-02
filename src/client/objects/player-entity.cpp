#include "pch.h"
#include "player-entity.h"

#include <framework/application.h>

#include "../messages/move-message.h"
#include "../messages/look-message.h"
#include "../messages/upstatus-message.h"

#include "../services/client-service.h"
#include "messages/fire-message.h"

PlayerEntity::PlayerEntity(OwnerID _owner, Vector3 _position)
    : NetworkEntity(_owner, ModelType::SHIP, _position) {
    ClientService& client = APPLICATION.getService<ClientService>();
    client.AddMessageListener<MoveMessage>(this, &PlayerEntity::OnMove);
    client.AddMessageListener<LookMessage>(this, &PlayerEntity::OnLook);
    client.AddMessageListener<UpStatus_Message>(this , &PlayerEntity::OnRecall);
}

void PlayerEntity::OnSpawned() {
    NetworkEntity::OnSpawned();
    m_font.Create(16, CPU_WHITE, "Consolas");
}

void PlayerEntity::OnUpdate() {
    NetworkEntity::OnUpdate();
    
    ClientService& client = APPLICATION.getService<ClientService>();
    
    if(getOwner() != client.getOwner())
        return;
    
    XMFLOAT3 dir = {0,0,0};
    bool input = false;
    if (cpuInput.IsKey('W') || cpuInput.IsKey('Z') || cpuInput.IsKey(VK_UP) ) {
        dir.z += 1;
        input = true;
    }
    if (cpuInput.IsKey('S') || cpuInput.IsKey(VK_DOWN)) {
        dir.z -= 1;
        input = true;
    }
    if (cpuInput.IsKey('A') || cpuInput.IsKey('Q') || cpuInput.IsKey(VK_LEFT)) {
        dir.x -= 1;
        input = true;
    }
    if (cpuInput.IsKey('D') || cpuInput.IsKey(VK_RIGHT)) {
        dir.x += 1;
        input = true;
    }
    
    if (cpuInput.IsKey(VK_SPACE) || cpuInput.IsKey(XBUTTON1)) {
        FireMessage fire; // No data, just send an event to the server to tell we want to fire
        client.sendMessage(fire);
    }
    
    MoveMessage movement;
    movement.dirX = dir.x;
    movement.dirY = dir.y;
    movement.dirZ = dir.z;
    
    client.sendMessage(movement);
    
    cpu_ray ray;
    cpuEngine.GetCursorRay(ray);
    float t = -20 / ray.dir.y;
    XMFLOAT3 pos = {
        ray.dir.x * t,
        ray.dir.z * t,
        0.001f
    };
    
    LookMessage look;
    look.posX = pos.x;
    look.posY = pos.y;
    look.posZ = pos.z;
    
    client.sendMessage(look);
}

void PlayerEntity::OnDestroy() {
    NetworkEntity::OnDestroy();
}
void PlayerEntity::OnRender(int _pass) {
    NetworkEntity::OnRender(_pass);

    ClientService& client = APPLICATION.getService<ClientService>();



    switch (_pass) {
    case CPU_PASS_PARTICLE_BEGIN:
        break;
    case CPU_PASS_PARTICLE_END:
        break;
    case CPU_PASS_UI_END:
    {
        std::string score = "Score: " + std::to_string(m_score);
        std::string health = "Health: " + std::to_string(m_health);
		std::string username = m_username;
        float pixelsPerUnit =
            (cpuDevice.GetHeight() * 0.5f) /
            (tanf(cpuEngine.GetCamera()->fov * 0.5f) * cpuEngine.GetCamera()->transform.pos.y);

        float screenX =
            cpuDevice.GetWidth() * 0.5f +
            m_entity->transform.pos.x * pixelsPerUnit;

        float screenY =
            cpuDevice.GetHeight() * 0.5f -
            m_entity->transform.pos.z * pixelsPerUnit;

        cpuDevice.DrawText(&m_font, health.c_str(), screenX, screenY, CPU_TEXT_CENTER);
        cpuDevice.DrawText(&m_font, score.c_str(), screenX, screenY + 11, CPU_TEXT_CENTER);
		cpuDevice.DrawText(&m_font, username.c_str(), screenX, screenY - 11, CPU_TEXT_CENTER);
        if (this->getOwner() != client.getOwner())
            return;
        std::string coord = "Coords: \nx: " + std::to_string(m_entity->transform.pos.x) +
            "\ny: " + std::to_string(m_entity->transform.pos.y);
        cpuDevice.DrawText(&m_font, coord.c_str(), (int)(cpuDevice.GetWidth() * 0.08f), 10, CPU_TEXT_CENTER);
        break;
    }
    default:
        break;
    }
}

void PlayerEntity::OnMove(MoveMessage* _message, OwnerID _useless) {
    if (_message->owner != getOwner())
        return;
    
    m_entity->transform.SetPosition(_message->posX, _message->posY, _message->posZ);
}

void PlayerEntity::OnLook(LookMessage* _message, OwnerID _owner) {
    if (_message->owner != getOwner())
        return;
    
    m_entity->transform.quat = XMFLOAT4{ _message->rotX, _message->rotY, _message->rotZ, _message->rotW };
    m_entity->transform.SetRotationFromQuaternion();
}
void PlayerEntity::OnRecall(UpStatus_Message* _message, OwnerID _owner) {
    
    if (_message->ownerid != getOwner())
        return;
    
    if (_message->health <= 0) {
        m_entity->visible = false;
    }
    else {
        m_entity->visible = true;
    }
	m_username = _message->username;
    m_health = _message->health;
    m_score = _message->score;
}