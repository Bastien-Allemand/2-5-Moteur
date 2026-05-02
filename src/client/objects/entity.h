#pragma once

#include "model.h"

#include <utils/maths/vector.h>

class Entity {
public:
    Entity(
        const Model& _model,
        Vector3 _position = Vector3::zero(),
        Vector3 _rotation = Vector3::zero(),
        Vector3 _scale = Vector3::one()
    );
    
    Entity(
        ModelType _model,
        Vector3 _position = Vector3::zero(),
        Vector3 _rotation = Vector3::zero(),
        Vector3 _scale = Vector3::one()
    );
    
    virtual ~Entity();
    
    void setModel(const Model& _model);
    Model& getModel() const;
    
    void setPosition(const Vector3& _position);
    const Vector3& getPosition() const;
    
    void setRotation(const Vector3& _rotation);
    const Vector3& getRotation() const;
    
    void setScale(const Vector3& _scale);
    const Vector3& getScale() const;
    
    virtual void OnSpawned()            {};
    virtual void OnUpdate()             {};
    virtual void OnDestroy()            {};
    virtual void OnRender(int _pass)    {};
    
    void destroy();

protected:
    mutable Model m_model;
    cpu_entity* m_entity;
};