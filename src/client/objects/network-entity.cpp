#include "pch.h"
#include "network-entity.h"

NetworkEntity::NetworkEntity(
    OwnerID _owner,
    const Model& _model,
    Vector3 _position,
    Vector3 _rotation,
    Vector3 _scale
    ) : Entity(_model, _position, _rotation, _scale), m_owner(_owner) {}

NetworkEntity::NetworkEntity(
    OwnerID _owner,
    ModelType _model,
    Vector3 _position,
    Vector3 _rotation,
    Vector3 _scale
    ) : Entity(_model, _position, _rotation, _scale), m_owner(_owner) {}


OwnerID NetworkEntity::getOwner() const {
    return m_owner;
}
