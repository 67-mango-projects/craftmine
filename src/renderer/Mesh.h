#pragma once
#include "Texture.h"
#include "../core/Math.h"
#include "Renderer.h"




class Mesh {
    TextureFrame m_texture;
    Vertex2 m_data[4] = { // Default Quad Data
        Vertex2(
            -1.0f,  1.0f, // pos
            0.0,1.0, // texture pos
            1.0, 0, 0.0 // color
        ),
        Vertex2(
            -1.0f, -1.0f,
            0.0,0.0,
            0.0, 0.0, 1.0
        ),
        Vertex2(
            1.0f,  1.0f,
            1.0,1.0,
            0.0, 1.0, 0
        ),
        Vertex2(
            1.0f, -1.0f,
            1.0,0.0,
            1.0, 1.0, 1.0
        )
    };
    unsigned int m_indices[6]{
        0,1,2,
        2,3,1
    };

    RendererInfo* m_renderInfo;
public:
    Mesh(TextureFrame texture, const Vertex2(&data)[4], VertexBufferLayout vbl);
    RendererInfo* getRendererInfo() const { return m_renderInfo; }
};