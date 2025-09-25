#pragma once
#include "../core/Camera.h"
#include "Mesh.h".h"

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
    
    
public:
    QuadRenderer();
    void loadMeshesFromTexture(Texture t);
};