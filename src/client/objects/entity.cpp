#include "pch.h"
#include "entity.h"

#include "framework/application.h"
#include "services/render-service.h"

Entity::Entity(const Model& _model, Vector3 _position, Vector3 _rotation, Vector3 _scale) {
    m_entity = cpuEngine.CreateEntity();
    setModel(_model);
    m_entity->transform.SetPosition(_position.x, _position.y, _position.z);
    m_entity->transform.SetYPR(_rotation.x, _rotation.y, _rotation.z);
    m_entity->transform.SetScaling(_scale.x);
}

Entity::Entity(ModelType _model, Vector3 _position, Vector3 _rotation, Vector3 _scale) {
    m_entity = cpuEngine.CreateEntity();
    setModel(Model(_model));
    m_entity->transform.SetPosition(_position.x, _position.y, _position.z);
    m_entity->transform.SetYPR(_rotation.x, _rotation.y, _rotation.z);
    m_entity->transform.SetScaling(_scale.x);
}

Entity::~Entity() {
    cpuEngine.Release(m_entity);
}

void Entity::setModel(const Model& _model) {
    m_model = _model;
    m_entity->pMesh = &m_model.getMesh();
}

Model& Entity::getModel() const {
    return m_model;
}

void Entity::setPosition(const Vector3& _position) {
    m_entity->transform.SetPosition(_position.x, _position.y, _position.z);
}

const Vector3& Entity::getPosition() const {
    Vector3 position;
    
    position.x = m_entity->transform.pos.x;
    position.y = m_entity->transform.pos.y;
    position.z = m_entity->transform.pos.z;
    
    return position;
}

void Entity::setRotation(const Vector3& _rotation) {
    m_entity->transform.SetYPR(_rotation.x, _rotation.y, _rotation.z);
}

const Vector3& Entity::getRotation() const {
    Vector3 rotation;
    
    rotation.x = m_entity->transform.pos.x;
    rotation.y = m_entity->transform.pos.y;
    rotation.z = m_entity->transform.pos.z;
    
    return rotation;
}

void Entity::setScale(const Vector3& _scale) {
    m_entity->transform.SetScaling(_scale.x);
}

const Vector3& Entity::getScale() const {
    Vector3 scale;
    
    scale.x = m_entity->transform.sca.x;
    scale.y = m_entity->transform.sca.y;
    scale.z = m_entity->transform.sca.z;
    
    return scale;
}
void Entity::destroy() {
    RenderService& render = APPLICATION.getService<RenderService>();
    render.destroy(this);
}