#pragma once

#include "network-entity.h"

struct UpStatus_Message;
struct MoveMessage;
struct LookMessage;

class PlayerEntity : public NetworkEntity {
public:
    PlayerEntity(OwnerID _owner, Vector3 _position = Vector3::zero());
    
protected:
    void OnSpawned() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void OnRender(int _pass) override;
    
    void OnMove(MoveMessage* _message, OwnerID _owner);
    void OnLook(LookMessage* _message, OwnerID _owner);
    void OnRecall(UpStatus_Message* _message,OwnerID _owner);
    
    int m_health;
    int m_score;
	std::string m_username;

private:
    cpu_font m_font;
};