#pragma once
#include "../core/Camera.h"
#include "Mesh.h".h"
#include<unordered_map>
struct Quad {
    Vector2 position = Vector2(0, 0);
    Vector2 size = Vector2(0, 0);
    float rotation = 0;
    Mesh* mesh;

    inline Quad(Mesh mesh, Vector2 position, Vector2 size) {
        this->mesh = &mesh;
        this->position = position;
        this->size = size;
    }
};

class QuadRenderer {
private:
    inline static std::unordered_map<std::string,Mesh> m_meshes;
    
public:
    QuadRenderer();
    void loadMeshesFromTexture(Texture t);
    static Mesh getMesh(std::string name);
};