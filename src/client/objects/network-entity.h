#pragma once

#include "entity.h"

#include <framework/networking/owner.h>

class NetworkEntity : public Entity {
public:
    NetworkEntity(
        OwnerID _owner,
        const Model& _model,
        Vector3 _position = Vector3::zero(),
        Vector3 _rotation = Vector3::zero(),
        Vector3 _scale = Vector3::one()
    );
    
    NetworkEntity(
        OwnerID _owner,
        ModelType _model,
        Vector3 _position = Vector3::zero(),
        Vector3 _rotation = Vector3::zero(),
        Vector3 _scale = Vector3::one()
    );
    
    OwnerID getOwner() const;
    
protected:
    OwnerID m_owner;
};