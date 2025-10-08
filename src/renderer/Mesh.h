#pragma once
#include "Texture.h"
#include "../core/Math.h"
#include "Renderer.h"
#include <vector>

// holy fuck meshes game me so much bugs, remember to keep vertex positions consistent

class Mesh {
    TextureFrame m_texture;
    Vertex2 m_data[4];

    unsigned int m_indices[6];

    std::vector<Vertex2> m_dataVec;
    std::vector<unsigned int> m_indicesVec;
    inline static Mesh* m_boxMesh;
    RendererInfo* m_renderInfo;
public:
    
    static void initBoxMesh();
    static const Mesh* getBoxMesh() {
        return m_boxMesh;
    };
    Mesh() {}
    Mesh(TextureFrame texture, const Vertex2(&data)[4], VertexBufferLayout vbl,unsigned int indices[6]);
    Mesh(const std::vector<Vertex2>& vertices,
        const std::vector<unsigned int>& indices,
        VertexBufferLayout vbl);
    RendererInfo* getRendererInfo() const { return m_renderInfo; }
};