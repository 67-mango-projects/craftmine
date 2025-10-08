#pragma once
#include "../core/Camera.h"
#include "Mesh.h"
#include<unordered_map>
struct Quad {
    Vector2 position = Vector2(0, 0);
    Vector2 size = Vector2(0, 0);
    Vector4 color = Vector4(1,1, 1, 1);
    float rotation = 0;
    const Mesh* mesh;
    float zIndex = 0;

    inline Quad(const Mesh* mesh, Vector2 position, Vector2 size) {
        this->mesh = mesh;
        this->position = position;
        this->size = size;
    }

    inline Quad(const Mesh* mesh, Vector2 position, Vector2 size,float zIndex) {
        this->mesh = mesh;
        this->position = position;
        this->size = size;
        this->zIndex = zIndex;
    }
    glm::mat4 getModelMatrix() const;


    bool isCollidingWith(const Quad& other) const;
    bool isCollidingWith_NoRotation(const Quad& other) const {
        Vector2 halfA = size * 0.5f;
        Vector2 halfB = other.size * 0.5f;

        return (position.x - halfA.x < other.position.x + halfB.x &&
            position.x + halfA.x > other.position.x - halfB.x &&
            position.y - halfA.y < other.position.y + halfB.y &&
            position.y + halfA.y > other.position.y - halfB.y);
    }
    bool containsPoint(const Vector2& point) const {
        Vector2 half = size * 0.5f;
        Vector2 min = position - half;
        Vector2 max = position + half;

        return (point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y);
    }


};

class Shader;
class QuadRenderer {
private:
    inline static std::unordered_map<std::string,Mesh*> m_meshes;
    Renderer* m_renderer;
public:
    Shader* worldshader;
    Texture* worldtexture;

    QuadRenderer(Renderer* renderer) : m_renderer(renderer) {};
    Vector2 worldToScreen(const Vector2& world);
    void render(const Quad*, bool usingTexture = true) const;
    void renderOnScreen(const Quad*, Shader* shader) const;
    void loadMeshesFromTexture(Texture *t);
    static const Mesh* getMesh(std::string name);
};