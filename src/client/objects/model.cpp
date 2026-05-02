#include "pch.h"
#include "model.h"

Model::Model(ModelType _type, Color _color) {
    setModelType(_type);
    setColor(_color);
}

void Model::setModelType(ModelType _type) {
    switch (_type) {
    case ModelType::CUBE:
        m_mesh.CreateCube();
        break;
    case ModelType::SPHERE:
        m_mesh.CreateSphere();
        break;
    case ModelType::SHIP:
        m_mesh.CreateSpaceship();
        break;
    default:
        break;
    }
    m_type = _type;
}
ModelType Model::getModelType() const {
    return m_type;
}

void Model::setColor(Color _color) {
    m_material.color.x = _color.r / 255.0f;
    m_material.color.y = _color.g / 255.0f;
    m_material.color.z = _color.b / 255.0f;
}
Color& Model::getColor() const {
    Color color;
    
    color.r = static_cast<byte>(m_material.color.x  * 255.0f);
    color.g = static_cast<byte>(m_material.color.y  * 255.0f);
    color.b = static_cast<byte>(m_material.color.z  * 255.0f);
    
    return color;
}

void Model::setMesh(cpu_mesh& _mesh) {
    m_mesh = _mesh;
}
cpu_mesh& Model::getMesh() const {
    return m_mesh;
}

void Model::setMaterial(cpu_material& _material) {
    m_material = _material;
}
cpu_material& Model::getMaterial() const {
    return m_material;
}