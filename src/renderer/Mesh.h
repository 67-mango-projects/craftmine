#pragma once
#include "Texture.h"
#include "../core/Math.h"
#include "Renderer.h"




class Mesh {
    TextureFrame m_texture;
    Vertex2 m_data[4];

    unsigned int m_indices[6]{
        0,1,2,
        2,3,1
    };

    RendererInfo* m_renderInfo;
public:
    Mesh() {}
    Mesh(TextureFrame texture, const Vertex2(&data)[4], VertexBufferLayout vbl);
    RendererInfo* getRendererInfo() const { return m_renderInfo; }
};