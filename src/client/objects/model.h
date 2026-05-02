#pragma once

#include <utils/types.h>

enum class ModelType {
    CUBE,
    SPHERE,
    SHIP
};

struct Color {
    byte r;
    byte g;
    byte b;
    
    inline static const Color& white() {
        return Color(255, 255, 255);
    }
    
    inline static const Color& red() {
        return Color(255, 0, 0);
    }
};

class Model {
public:
    Model(ModelType _type = ModelType::CUBE, Color _color = Color::white());
    
    void setModelType(ModelType _type);
    ModelType getModelType() const;
    
    void setColor(Color _color);
    Color& getColor() const;
    
    void setMesh(cpu_mesh& _mesh);
    cpu_mesh& getMesh() const;
    
    void setMaterial(cpu_material& _material);
    cpu_material& getMaterial() const;

private:
    ModelType m_type;
    mutable cpu_mesh m_mesh;
    mutable cpu_material m_material;
};